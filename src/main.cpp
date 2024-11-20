#include <iostream>
#include <sstream>
#include <vector>
#include <bitset>
#include <iomanip>
#include <string>
#include <utility>
#include <cstdint>
#include <iomanip>
#include <numeric>
#include <fstream>
#include <algorithm>
#include <limits>
#include <deque>

struct Instruction {
	// 各フィールドをビットフィールドとして定義
	std::string code;
	unsigned int opcode : 4;   // 4ビットのオペコード
	unsigned int rdest : 4;     // 4ビットのレジスタ1
	unsigned int rsrc : 4;     // 4ビットのレジスタ2
	std::int8_t imm8;          // 8ビットの符号付き整数
	std::int16_t read_rdest;
	std::int16_t read_rsrc;

	// imm8を符号拡張して16ビットの整数として返すメソッド
	std::int16_t getExtendedImmediate() const {
		return static_cast<std::int16_t>(imm8);
	}
};

// グループで変更されている可能性あり。適宜調整お願いします
enum Opcode {
	//! この順番が保証されなければ動作しません
	NOP  = 0x0,
	ADD  = 0x1,
	SUB  = 0x2,
	AND  = 0x3,
	OR   = 0x4,
	ADDI = 0x5,
	SUBI = 0x6,
	LDI  = 0x7
};

Opcode stringToOpcode(const std::string& str) {
	if (str == "NOP") return NOP;
	else if (str == "ADD" || str == "add") return ADD;
	else if (str == "SUB" || str == "sub") return SUB;
	else if (str == "AND" || str == "and") return AND;
	else if (str == "OR" || str == "or") return OR;
	else if (str == "ADDI" || str == "addi") return ADDI;
	else if (str == "SUBI" || str == "subi") return SUBI;
	else if (str == "LDI" || str == "ldi") return LDI;
	else throw std::runtime_error("Unknown opcode: " + str);
}


// 命令フォーマットを生成する関数
std::string generateInstruction2bit(std::int16_t instruction) {

	std::bitset<16> binary(instruction);

	// 結果を戻す
	return " 2進数バイナリ:\t[ " + binary.to_string().insert(4, " ").insert(9, " ").insert(14, " ") + " ]";
}

std::string generateInstruction16bit(std::int16_t instruction) {
	// 16進数フォーマットに変換
	std::stringstream hexStream;
	hexStream << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << instruction;

	return "16進数バイナリ:\t[ " + hexStream.str() + " ]";
}

void fetch(Instruction& inst, std::vector<std::string> words) {
	// オペコードの取得
	unsigned int opcode = 0;   // 4ビットのオペコード
	unsigned int rdest = 0;     // 4ビットのレジスタ1
	unsigned int rsrc = 0;     // 4ビットのレジスタ2
	std::int8_t imm8 = 0;          // 8ビットの符号付き整数

	if (words.size() == 1 && stringToOpcode(words[0]) == NOP) {
		inst.opcode = 0;
		inst.rdest = 0;
		inst.rsrc = 0;
		inst.imm8 = 0;
		return;
	}

	// 命令コマンドの長さが適切でなければエラーを飛ばす
	if (words.size() < 2)
		throw std::runtime_error("Invalid instruction format");
	// オペコードが存在するかチェック
	opcode = stringToOpcode(words[0]);

	std::string regst = words[1].substr(0, 1);

	// regの取得R1, R2...// 参照先が存在するかチェック
	if (regst == "R" || regst == "r") rdest = std::stoi(words[1].substr(1)); // 整数に変換
	// 一致しなければ名前エラーを飛ばす
	else throw std::runtime_error("Unknown register: " + words[1]);

	regst = words[2].substr(0, 1);

	// オペコードによって処理を変える
	// ADD, SUB, AND, OR
	if (opcode >= ADD && opcode <= OR) {
		// reg2の取得
		if (regst == "R" || regst == "r") {
			rsrc = std::stoi(words[2].substr(1));
		} else {
			throw std::runtime_error("Unknown register: " + words[2]);
		}
	} else if (opcode <= LDI) {
		// imm8の取得
		imm8 = std::stoi(words[2]);

	}

	// オペコード4ビット、レジスタ1とレジスタ2それぞれ4ビットの命令を作成
	std::int16_t instruction;

	// imm8を0-255の範囲に制限する（8ビットに収める）

	if (opcode >= ADD && opcode <= OR) {
		instruction = (opcode << 12) | (rdest << 8) | (rsrc << 4);
	} else if (opcode <= LDI) {
		instruction = (opcode << 12) | (rdest << 8) | (imm8 & 0xFF); // imm8を下位8ビットに格納
	} else {
		instruction = 0;
	}

	inst.opcode = (instruction >> 12) & 0xF;
	inst.rdest = (instruction >> 8) & 0xF;
	inst.rsrc = (instruction >> 4) & 0xF;
	inst.imm8 = instruction & 0xFF;

	std::cout << "フェッチ結果↓\n";
	std::cout << generateInstruction2bit(instruction) << std::endl;
	std::cout << generateInstruction16bit(instruction) << "\n" << std::endl;
}

std::string printResultDecode(Instruction& inst) {
	std::stringstream hexStream1;
	std::stringstream hexStream2;

	if (inst.opcode >= ADD && inst.opcode <= OR) {
		hexStream1 << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << inst.read_rdest;
		hexStream2 << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << inst.read_rsrc;

		return "R" + std::to_string(inst.rdest) + "\t: " + hexStream1.str() + " (" + std::to_string(inst.read_rdest) + ")" + "\n" + "R" + std::to_string(inst.rsrc) + "\t: " + hexStream2.str() + " (" + std::to_string(inst.read_rsrc) + ")" + "\n";
	} else if (inst.opcode <= LDI) {
		hexStream1 << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << inst.read_rdest;
		hexStream2 << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << inst.getExtendedImmediate();
		
		return "R" + std::to_string(inst.rdest)  + "\t: " + hexStream1.str() + " (" + std::to_string(inst.read_rdest) + ")" + "\n" + "imm8\t: " + hexStream2.str() + " (" + std::to_string(inst.getExtendedImmediate()) + ")" + "\n";
	}

	return "NOP\n";
}

void decode(Instruction& inst, std::vector<std::int16_t> rom, std::pair<int, std::int16_t> pre_reg) {
	// レジスタの値を取得する
	if (inst.rdest == pre_reg.first) inst.read_rdest = pre_reg.second;
	inst.read_rdest = rom[inst.rdest];

	if (inst.opcode <= OR) inst.read_rsrc = rom[inst.rsrc];
	
	std::cout << "デコード結果↓\n";
	std::cout << printResultDecode(inst) << std::endl;
}

std::string printResultExecute(Instruction& inst) {
	// 16進数フォーマットに変換
	std::stringstream hexStream1;
	std::stringstream hexStream2;
	hexStream1 << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << inst.read_rdest;
	return "R" + std::to_string(inst.rdest) + "\t: " + hexStream1.str() + " (" + std::to_string(inst.read_rdest) + ")" + "\n";
}

void execute(Instruction& inst) {
	switch (inst.opcode) {
		case ADD:
			inst.read_rdest += inst.read_rsrc;
			break;
		case SUB:
			inst.read_rdest -= inst.read_rsrc;
			break;
		case AND:
			inst.read_rdest &= inst.read_rsrc;
			break;
		case OR:
			inst.read_rdest |= inst.read_rsrc;
			break;
		case ADDI:
			inst.read_rdest += inst.getExtendedImmediate();
			break;
		case SUBI:
			inst.read_rdest -= inst.getExtendedImmediate();
			break;
		case LDI:
			inst.read_rdest = inst.getExtendedImmediate();
			break;
		default:
			// 不明なオペコードの処理
			break;
	}

	std::cout << "実行結果↓\n";
	std::cout << printResultExecute(inst) << std::endl;
}

std::string printResultWriteBack(std::vector<std::int16_t> rom) {
	std::stringstream hexStream;
	std::string result = "";
	for (size_t i = 0; i < rom.size(); ++i) {
		hexStream << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << rom[i];
		result += "R" + std::to_string(i) + "\t: " + hexStream.str() + " (" + std::to_string(rom[i]) + ")" + "\n";
		hexStream.str("");
		hexStream.clear();
	}
	return result;
}

void writeBack(Instruction& inst, std::vector<std::int16_t>& rom) {
	rom[inst.rdest] = inst.read_rdest;
	std::cout << "書き込み結果↓\n";
	std::cout << printResultWriteBack(rom) << std::endl;
}

int main() {
	// ADD命令のオペコードとレジスタ
	std::string code;
	std::vector<std::string> words;
	Instruction inst;

	std::vector<std::int16_t> rom(16, 0);

	std::cout << "レジスタの初期化が完了しました\n";

	std::cout << "命令を入力してください(qで終了):\n";

	std::pair<int, std::int16_t> pre_reg = {0, 0};

	while (true) {
		std::cout << "==========" << std::endl;
		getline(std::cin, code);
		std::cout << "==========" << std::endl;

		if (code == "q") break;
		else inst.code = code;

		// 単語の分解
		std::istringstream stream(code);
		std::string word;
		while (stream >> word) {
			words.push_back(word);
		}

		try {
			fetch(inst, words);

			decode(inst, rom, pre_reg);

			execute(inst);

			writeBack(inst, rom);

		} catch (const std::exception& e) {
			std::cerr << "命令解読中にエラーが発生しました: " << e.what() << std::endl;
		}
		words.clear();
	}

	return 0;
}