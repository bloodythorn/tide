#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

#include <tide/games/checkers/checkers.hpp>
#include <tide/engine/logger.hpp>
#include <tide/engine/options.hpp>

using namespace tide::Engine;
using namespace tide::Games::Checkers;

/* Module Name */
static const std::wstring MAIN{L"Entry_Point"};

/* Data Structure for Command */
using CommandFunc =
  std::function<bool(Checkers&, const std::vector<std::string>&)>;
struct CommData {
  CommandFunc exe;
  uint8_t argNum;
  CommData(CommandFunc fn, uint8_t c=0)
  : exe(fn), argNum(c) {}
};
using CommandMap = std::unordered_map<std::string, CommData>;

/* Commands */
static const CommandMap COMMANDS {
  { "-ng" ,
    CommData([](Checkers& c, Options::Args a){ return c.newGame(); } )
  },
  { "-lg" ,
    CommData([](Checkers& c, Options::Args a){ return c.load(a[0]); } , 1 )
  },
  { "-sg",
    CommData(
      [](Checkers& c, Options::Args a){
        LOG_INFO(L"COMMAND:-sg") << "Game Saved: " << c.save().c_str();
        return true;
      } )
  },
  { "-lm",
    CommData(
      [](Checkers& c, Options::Args a){
        LOG_INFO(L"COMMAND:-lm") << "Valid Moves:\n"
          << c.listMoves().c_str();
        return true;
      } )
  },
  { "-lr",
    CommData(
      [](Checkers& c, Options::Args a){
        LOG_INFO(L"COMMAND:-lr") << "Valid Backward Moves:\n"
          << c.rlistMoves().c_str();
        return true;
      } )
  },
  { "-mm",
    CommData(
      [](Checkers& c, Options::Args a){
        return c.move(std::stoi(a[0]), std::stoi(a[1]));
      } , 2 )
  },
  { "-rm",
    CommData(
      [](Checkers& c, Options::Args a){
        return c.rmove(std::stoi(a[0]), std::stoi(a[1]));
      } , 2 )
  },
};

/* Execute each instruction in order */
bool execute(const Options::Option& op, Checkers& ch) {

  /* Find the command */
  auto a = COMMANDS.end();
  if(((a = COMMANDS.find(op.name)) != COMMANDS.end())) {

    /* Validate Argument count & execute */
    if(op.args.size() == a->second.argNum) return a->second.exe(ch, op.args);

    /* Arg count doesn't match */
    else {
      LOG_FATAL(MAIN) << Log::ff(__func__)
        << "Incorrect arguments given for" << a->first.c_str() << "."
        << ", expected " << a->second.argNum << ".";
      return false;
    }

  /* Command not found */
  } else {
    LOG_FATAL(MAIN) << Log::ff(__func__)
      << "Unknown Command:" << op.name.c_str();
    return false;
  }
}

int main(int argc, char** argv) {

  /* Main execution and output */
  try {
    Options::ArgParser ap{argc, argv};
    Checkers cg;

    /* No comm/args nothing to do. */
    if(ap.size() == 0) {
      LOG_INFO(MAIN) << Log::ff(__func__)
        << "No commands given. Exiting.";
        exit(EXIT_SUCCESS);
    }

    /* Execute commands given in order */
    while(!ap.empty()) {
      auto cmd = ap.front(); ap.pop();
      if(!execute(cmd, cg)) {
        LOG_FATAL(MAIN) << Log::ff(__func__)
          << "failed to execute command: " << cmd.name.c_str();
        exit(EXIT_FAILURE);
      }
    }

    /* Output Results */
    std::cout << cg << std::endl;

  /* Catch General Exceptions */
  } catch(const std::exception& e) {
    LOG_FATAL(MAIN) << Log::ff(__func__)
      << "Catch e:" << e.what();
    std::exit(EXIT_FAILURE);

  /* Catch Anything Else */
  } catch(...) {
    LOG_FATAL(MAIN) << Log::ff(__func__)
      << "Unhanded Exception";
    std::exit(EXIT_FAILURE);
  }

  /* Exit Cleanly */
  std::exit(EXIT_SUCCESS);
}