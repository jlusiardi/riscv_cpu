#include <iostream>
#include <bitset>
#include <fstream>

#include "verilated.h"
#include "Vcpu.h"
#include "general_test.h"
#include "riscv.h"
#include "cxxopts.hpp"

using namespace std;

#define ROM cpu__DOT__mem__DOT__rom___DOT__rom
#define RAM cpu__DOT__mem__DOT__ram__DOT__mem
#define PC cpu__DOT__pc_register__DOT__Q_data
#define STAGE cpu__DOT__stage_counter__DOT__data
#define REGISTERS cpu__DOT__register_file__DOT__registers

class RunRom: public GeneralTest<Vcpu> {
  private:
	string filename;
  public:
  	RunRom(string filename): filename(filename) {}
	
    void load_rom(string filename) {
      ifstream myfile;
      char * memblock;
      streampos size;

      myfile.open(filename, ios::binary|ios::ate);
      size = myfile.tellg();
      memblock = new char[size];
      myfile.seekg(ios::beg);
      myfile.read(memblock, size);

      for (int p = 0; p < size; p++) {
        top->ROM[p] = memblock[p];
      }
      delete[] memblock;
    }

    virtual void test() {
      load_rom(this->filename);

      step();
      top->rst = 1;

      uint32_t pc_old = -1;
      while(top->PC != pc_old) {
        pc_old = top->PC;
        clock_cycles(STAGE_COUNT);
      }
//      ASSERT_EQUALS(top->RAM[0], 2);
//	  ASSERT_EQUALS(top->RAM[4], 3);
//      ASSERT_EQUALS(top->RAM[8], 233);
//      ASSERT_EQUALS(read_4bytes(top->RAM, 12), 377);
    }
};

int main(int argc, char* argv[]) {
	cxxopts::Options options("RunRom", "Runs a rom and performs some tests");
	options.add_options()
		("help", "Print help")
  		("t,trace", "Enable writing a vcd trace", cxxopts::value<bool>()->default_value("false"))
  		("f,file", "File name of the ROM", cxxopts::value<std::string>());
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

	cout << "---- Testing " << romfile << endl;
	(new RunRom(romfile))->run(vcdfile.c_str(), do_trace);
	cout << "$$$$ " << romfile << " tests passed" << endl;
 	HANDLE_ERROR_COUNTER;
}
