/**
 * @file The FakeDataProducerDAQModule class interface
 *
 * FakeDataProducerDAQModule is a simple DAQModule implementation that simply
 * logs the fact that it received a command from DAQProcess.
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef APP_FRAMEWORK_INCLUDE_APP_FRAMEWORK_DAQModuleS_FAKEDATAPRODUCERDAQModule_HH_
#define APP_FRAMEWORK_INCLUDE_APP_FRAMEWORK_DAQModuleS_FAKEDATAPRODUCERDAQModule_HH_

#include "app-framework-base/Buffers/Buffer.hh"
#include "app-framework-base/DAQModules/DAQModule.hh"
#include "app-framework-base/DAQModules/DAQModuleThreadHelper.hh"

#include <future>
#include <memory>
#include <string>
#include <vector>

namespace appframework {
/**
 * @brief FakeDataProducerDAQModule creates vectors of ints and sends them
 * downstream
 */
class FakeDataProducerDAQModule : public DAQModule {
public:
  explicit FakeDataProducerDAQModule(
      std::shared_ptr<BufferInput<std::vector<int>>> outputBuffer);

  std::future<std::string> execute_command(std::string cmd) override;

  FakeDataProducerDAQModule(const FakeDataProducerDAQModule &) = delete;
  FakeDataProducerDAQModule &
  operator=(const FakeDataProducerDAQModule &) = delete;
  FakeDataProducerDAQModule(FakeDataProducerDAQModule &&) = delete;
  FakeDataProducerDAQModule &operator=(FakeDataProducerDAQModule &&) = delete;

private:
  // Commands
  std::string do_configure();
  std::string do_start();
  std::string do_stop();

  // Threading
  DAQModuleThreadHelper thread_;
  void do_work();

  // Configuration
  std::shared_ptr<BufferInput<std::vector<int>>> outputBuffer_;
  std::chrono::milliseconds bufferTimeout_;
  size_t nIntsPerVector_;
  int starting_int_;
  int ending_int_;

  size_t wait_between_sends_ms_;
};
} // namespace appframework

#endif // APP_FRAMEWORK_INCLUDE_APP_FRAMEWORK_DAQModuleS_FAKEDATAPRODUCERDAQModule_HH_
