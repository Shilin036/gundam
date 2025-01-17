//
// Created by Nadrino on 21/04/2021.
//

#include "JsonUtils.h"
#include "GundamGreetings.h"

#include "Logger.h"
#include "CmdLineParser.h"
#include "GenericToolbox.h"

#include "string"



LoggerInit([]{
  Logger::setUserHeaderStr("[gundamConfigForwarder.cxx]");
})

int main( int argc, char** argv ){

  GundamGreetings g;
  g.setAppName("ConfigForwarder");
  g.hello();

  CmdLineParser clp(argc, argv);
  clp.addOption("config-file", {"-c"}, "Provide YAML/Json configuration file.", 1);
  clp.addOption("output-file-path", {"-o"}, "Set output file name.", 1);

  LogInfo << "Available options: " << std::endl;
  LogInfo << clp.getConfigSummary() << std::endl;

  clp.parseCmdLine();

  LogWarning << "Command line options:" << std::endl;
  LogWarning << clp.getValueSummary() << std::endl;

  LogInfo << "Reading config..." << std::endl;
  auto configFilePath = clp.getOptionVal<std::string>("config-file");
  auto fConfigFilePath = clp.getOptionVal<std::string>("output-file-path");
  fConfigFilePath += ".json";

  LogInfo << "Reading configuration file..." << std::endl;
  auto config = JsonUtils::readConfigFile(configFilePath);

  LogInfo << "Unfolding configuration file..." << std::endl;
  JsonUtils::unfoldConfig(config);
  std::stringstream ss;
  ss << config << std::endl;

  LogInfo << "Writing as: " << fConfigFilePath << std::endl;
  GenericToolbox::dumpStringInFile(fConfigFilePath, ss.str());

  LogInfo << "Unfolded config written as: " << fConfigFilePath << std::endl;

  g.goodbye();
  return EXIT_SUCCESS;
}
