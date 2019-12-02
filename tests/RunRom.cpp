#include <bitset>
#include <fstream>
#include <iostream>

#include "Vcpu.h"
#include "cxxopts.hpp"
#include "general_test.h"
#include "riscv.h"
#include "verilated.h"

using namespace std;

#define ROM cpu__DOT__mem_ctrl__DOT__mem__DOT__rom__DOT__mem
#define RAM cpu__DOT__mem_ctrl__DOT__mem__DOT__ram__DOT__mem
#define PC cpu__DOT__pc_register__DOT__Q_data
#define STAGE cpu__DOT__stage_counter__DOT__data
#define REGISTERS cpu__DOT__register_file__DOT__registers

class RunRom : public GeneralTest<Vcpu> {
private:
  string rom_file;
  string test_file;

public:
  RunRom(string rom_file, string test_file="") : rom_file(rom_file), test_file(test_file) {}

  void load_rom(string filename) {
    ifstream myfile;
    char *memblock;
    streampos size;

    myfile.open(filename, ios::binary | ios::ate);
    size = myfile.tellg();
    memblock = new char[size];
    myfile.seekg(ios::beg);
    myfile.read(memblock, size);

    for (int p = 0; p < size; p++) {
      top->ROM[p] = memblock[p];
    }
    delete[] memblock;
  }

  void perform_tests(string filename) {
    ifstream file;
    string line;
    file.open(filename);
    int linenumber=0;
    while(getline(file, line)) {
      linenumber++;
      int sep1 = line.find_first_of('@');
      int sep2 = line.find_first_of('=');

      string size = line.substr(0, sep1);
      int position = stoi(line.substr(sep1 + 1, sep2 - sep1 - 1));
      cout << "Executing test in line " << linenumber << ": " 
           << size << "@" << position << "=";
      if (size == "b") {
        signed char expected = stoi(line.substr(sep2 + 1));
        cout << expected << endl;
        ASSERT_EQUALS(read_byte(top->RAM, position), expected);
      } else if (size == "ub") {
        unsigned char expected = stoi(line.substr(sep2 + 1));
        cout << expected << endl;
        ASSERT_EQUALS(read_unsigned_byte(top->RAM, position), expected);
      } else if (size == "h") {
        signed short expected = stoi(line.substr(sep2 + 1));
        cout << expected << endl;
        ASSERT_EQUALS(read_halfword(top->RAM, position), expected);
      } else if (size == "uh") {
        unsigned short expected = stoi(line.substr(sep2 + 1));
        cout << expected << endl;
        ASSERT_EQUALS(read_unsigned_halfword(top->RAM, position), expected);
      } else if (size == "w") {
        signed int expected = stoi(line.substr(sep2 + 1));
        cout << expected << endl;
        ASSERT_EQUALS(read_word(top->RAM, position), expected);
      }
    }
  }

  virtual void test() {
    load_rom(this->rom_file);

    step();
    top->rst = 1;

    uint32_t pc_old = -1;
    // wait until the PC gets stationary ('while(while(1){})')
    while (top->PC != pc_old) {
      pc_old = top->PC;
      EXECUTE_INSTR;
    }
    if (this->test_file != "") {
      perform_tests(this->test_file);
    }
  }
};

int main(int argc, char *argv[]) {
  cxxopts::Options options("RunRom", "Runs a rom and performs some tests");
  options.add_options()
    ("help", "Print help")
    ("t,trace", "Enable writing a vcd trace", cxxopts::value<bool>()->default_value("false"))
    ("f,file", "File name of the ROM", cxxopts::value<std::string>())
    ("T,tests", "File name of the tests", cxxopts::value<std::string>());
  auto result = options.parse(argc, argv);
  if (result.count("help")) {
    std::cout << options.help({"", "Group"}) << std::endl;
    exit(0);
  }
  if (!result.count("f")) {
    std::cout << "Missing option for the ROM file!" << std::endl;
    std::cout << options.help({"", "Group"}) << std::endl;
    exit(0);
  }

  bool do_trace = result["t"].as<bool>();
  std::string romfile = result["f"].as<std::string>();
  std::string vcdfile = "vcds/" + result["f"].as<std::string>() + ".vcd";
  std::string testfile = result["T"].as<std::string>();

  cout << "---- Testing " << romfile << endl;
  (new RunRom(romfile, testfile))->run(vcdfile.c_str(), do_trace, true);
  cout << "$$$$ " << romfile << " tests passed" << endl;
  HANDLE_ERROR_COUNTER;
}
