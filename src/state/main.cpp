//
// Copyright (c) 2022 Alexander Sacharov <a.sacharov@gmx.de>
//               All rights reserved.
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.
//

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#include <getopt.h>
#include <syslog.h>

#include <chrono>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

#include "contextConcrete.hpp"
#include "fswatch.hpp"
#include "spdlog/spdlog.h"

using namespace std::chrono_literals;

//-----------------------------------------------------------------------------
// local Defines and Macros
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// local Typedefs, Enums, Unions
//-----------------------------------------------------------------------------

/**
 * @brief event plan structure
 */
struct Event {
 public:
  std::mutex event_mutex;
  std::condition_variable event_condition;
};

//-----------------------------------------------------------------------------
// local/global Variables Definitions
//-----------------------------------------------------------------------------
Event task_event_concrete;
Event task_event_filesystem;

//-----------------------------------------------------------------------------
// local/global Function Prototypes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// local Function Definitions
//-----------------------------------------------------------------------------

void WakeUpAllTasks();

/************************************************************************/ /**
* @fn      void ViewVersion(const char* prog)
* @brief   show version for winconfig.
* @param prog - name of program
****************************************************************************/
static void ViewVersion(const char* prog) {
  std::cout << prog << " v.1.0.0" << std::endl;
}

/************************************************************************/ /**
* @fn      void ViewHelp(const char* prog)
* @brief   view help
****************************************************************************/
static void ViewHelp(const char* prog) {
  std::cout << "Usage: " << prog << " [OPTION]\n"
            << "  -v, --version            version\n"
            << "  -h, --help               this message\n\n";
}

/************************************************************************/ /**
* @fn      void ProcessOptions(int argc, char* argv[])
* @brief   parse command line parameters
* @param argc - number parameters in command line
* @param argv - command line parameters as array
****************************************************************************/
static void ProcessOptions(int argc, char* argv[]) {
  for (;;) {
    int option_index = 0;
    static const char* short_options = "h?v";
    static const struct option long_options[] = {
        {"help", no_argument, 0, 0},
        {"version", no_argument, 0, 'v'},
        {0, 0, 0, 0},
    };

    int var = getopt_long(argc, argv, short_options, long_options, &option_index);

    if (var == EOF) {
      break;
    }
    switch (var) {
      case 0:
        ViewHelp(argv[0]);
        break;
      case '?':
      case 'h': {
        ViewHelp(argv[0]);
        exit(EXIT_SUCCESS);
      }
      case 'v': {
        ViewVersion(argv[0]);
        exit(EXIT_SUCCESS);
      }
      default: {
        ViewHelp(argv[0]);
        exit(-1);
      }
    }
  }
}

/**
* @brief   Termination code
*/
static void CloseAll() {}

/**
 * @brief   get char from cin
 * @return bool
 *          true if received an exit command,
 *          otherwise - false
 **/
static bool HandleGetChar() {
  char var = getchar();
  switch (var) {
    case '\n':
      break;
    case 'q': {
      std::cout << "Received QUIT command\nExiting.." << std::endl;
      return (true);
    }
    default: {
      std::cout << "Test Console \n"
                << " Key options are:\n"
                << "  q - quit from the program" << std::endl;
      break;
    }
  }
  return false;
}

//-----------------------------------------------------------------------------
// global Function Definitions
//-----------------------------------------------------------------------------

/**
 * @brief Waking up all running tasks
 * @desc  This wakes up all task
 * @param config - configuration the manager
 * @param all_tasks_wakeup - wakeup all tasks but not only file system event driving tasks
 */
void WakeUpRunningTasks(bool all_tasks_wakeup = true) {
  if (all_tasks_wakeup) {
    std::unique_lock lck(task_event_filesystem.event_mutex);
    task_event_filesystem.event_condition.notify_one();  // Wakes up stop a file system watcher
  }
  {
    std::unique_lock lck(task_event_concrete.event_mutex);
    task_event_concrete.event_condition.notify_one();  // Wakes up a displacement task
  }
}

/**
 * @brief File system watcher task main function
 * @param token - stop task token
 */
void TaskWorkerFsWatcher(std::stop_token token) {
  using namespace std::chrono_literals;
  const auto waitDuration = 2s;
  auto watcher = fswatch("/tmp");

  // add watching events
  watcher.on(fswatch::Event::FILE_CREATED, [&]([[maybe_unused]] auto& event) {
    WakeUpRunningTasks(false);  // Wake up sleeping tasks by an event in the file system
  });

  watcher.on(fswatch::Event::FILE_MODIFIED, [&]([[maybe_unused]] auto& event) {
    WakeUpRunningTasks(false);  // Wake up sleeping tasks by an event in the file system
  });

  watcher.on(fswatch::Event::FILE_DELETED, [&]([[maybe_unused]] auto& event) {
    WakeUpRunningTasks(false);  // Wake up sleeping tasks by an event in the file system
  });

  // Register a stop callback for stop task
  std::stop_callback stop_cb(token, [&]() {
    // Wake up thread on stop request
    task_event_filesystem.event_condition.notify_all();
  });

  // start thread with stop fs-watcher task
  std::thread stop_watching_task([&]() {
    // create observer
    while (true) {
      // Start of locked block
      std::unique_lock lck(task_event_filesystem.event_mutex);
      task_event_filesystem.event_condition.wait(lck, [&, token]() {
        return token.stop_requested();
      });

      //Stop if requested to stop
      if (token.stop_requested()) {
        spdlog::info("Stop requested for a stop watcher task");
        break;
      }
    }  // End of while loop
    watcher.stop();
    // wakeup watcher via event in file system
    std::ofstream tmpfile;
    tmpfile.open("/tmp/~watcher_wakeup", std::ios_base::trunc);
    tmpfile << "Wakeup\n";
    tmpfile.close();
    spdlog::info("Stop filesystem watcher task stopped.");
  });

  try {
    watcher.start();
  } catch (std::filesystem::filesystem_error& error) {
    spdlog::warn("Filesystem exception was caught: {}", error.what());
  } catch (std::exception& error) {
    spdlog::warn("Exception was caught: {}", error.what());
  } catch (...) {
    spdlog::warn("Unknown exception was caught");
  }

  stop_watching_task.join();

  spdlog::info("Filesystem watcher task stopped.");
}

/**
 * @brief Concrete context main function
 * @desc Threads cannot always actively monitor a stop token.
 * For instance, a thread waiting on a condition variable cannot check a stop condition unless it is signaled.
 * For that reason, a callback mechanism is provided through std::stop_callback.
 * A std::stop_callback instance registers a callback function for a given stop token.
 * The callback is invoked when the token receives a stop request.
 * The following shows how to use std::stop_callback to signal a thread waiting on a std::condition_variable
 * on a stop request:
 * @param token - stop task token
 */
void TaskWorker_Context(std::stop_token token) {
  using namespace std::chrono_literals;
  const auto waitDurationDef = 3000ms;

  // Register a stop callback
  std::stop_callback stop_cb(token, [&]() {
    // Wake thread on stop request
    task_event_concrete.event_condition.notify_all();
  });

  // create observer
  state::ConcreteContext observer;
  auto sooner = waitDurationDef;

  while (true) {
    // observe serves states
    sooner = observer.Serve(waitDurationDef);
    {
      // Start of locked block
      std::unique_lock lck(task_event_concrete.event_mutex);
      task_event_concrete.event_condition.wait_for(lck, std::chrono::milliseconds(sooner), [&, token]() {
        // Condition for wake up
        return token.stop_requested();
      });

      //Stop if requested to stop
      if (token.stop_requested()) {
        spdlog::info("Stop requested for a displacement connection task");
        break;
      }
    }  // End of locked block
  }    // End of while loop

  spdlog::info("Displacement connection task stopped.");
}

/************************************************************************/ /**
* @fn      int main()
* @brief   initializes and run stuff.
* @param   argc will be the number of strings pointed to by argv.
 * This will be 1 plus the number of arguments, as virtually all implementations
 * will prepend the name of the program to the array.
* @param   argv array of command line parameters.
*
* @return EXIT_SUCCESS if successfully, otherwise - EXIT_FAILURE
*
* @exception void
****************************************************************************/

int main(int argc, char** argv) {
  std::stop_source stop_src;  // Create a stop source
  std::thread task_worker_file_system;
  std::thread task_worker_context;

  //----------------------------------------------------------
  // parse parameters
  //----------------------------------------------------------
  ProcessOptions(argc, argv);

  // show information
  ViewVersion(argv[0]);

  //----------------------------------------------------------
  // go to idle in main
  //----------------------------------------------------------
  std::cout << " (type '?' for help)" << std::endl;

  // Create all workers and pass stop tokens
  task_worker_context = std::move(std::thread(TaskWorker_Context, stop_src.get_token()));
  // start task filesystem watcher
  task_worker_file_system = std::move(std::thread(TaskWorkerFsWatcher, stop_src.get_token()));

  // main loop or sleep
  while (true) {
    if (HandleGetChar()) {
      break;
    }
  }

  spdlog::info("Request stop all  tasks");
  // set token to stop all worker
  stop_src.request_stop();

  // wakeup all tasks
  WakeUpRunningTasks(true);

  // Join threads
  task_worker_file_system.join();
  task_worker_context.join();

  // Close all before to  exit
  CloseAll();

  return EXIT_SUCCESS;
}
