#include <iostream>
#include <queue>
#include <vector>
#include <random>
#include <unordered_map>
#include <string>

int getRand(int &, int &, std::mt19937 &);
std::vector<int> generate(int &, std::mt19937 &);
bool contains(std::unordered_map<int, bool> &, int &);
void clear(std::queue<int> &q);
void falsify(std::unordered_map<int, bool> &hash);

int main() {

	static std::random_device rd;
	static std::mt19937 mt(rd());
	auto seqSize = 1000;
	std::vector<int> sequence;
	std::unordered_map<int, bool> hash;
	std::queue<int> frames;
	int currPageFaults = 0;
	int prevPageFaults;
	unsigned int numSequences = 100;
	unsigned int maxFrames = 100;
	std::string anomalies= "";
	auto numAnomalies = 0;

	for (unsigned int seq = 1; seq <= numSequences; ++seq) {
		sequence = generate(seqSize, mt);
		prevPageFaults = 1000;
		for (unsigned int memSize = 1; memSize <= maxFrames; ++memSize) {
			falsify(hash);
			clear(frames);
			for (auto page : sequence) {
				auto test = hash[page];
				if (!test && frames.size() >= memSize) {
					++currPageFaults;
					hash.erase(frames.front());
					frames.pop();
					hash.insert({page, true});
					frames.push(page);
				}
				else if (!test && frames.size() < memSize) {
					++currPageFaults;
					hash.insert({page, true});
					frames.push(page);
				}
			}
			if (currPageFaults > prevPageFaults) {
				anomalies += "Anomaly Discovered!\n\tSequence: " + std::to_string(seq) +
					"\n\tPage Faults: " + std::to_string(prevPageFaults) + " @ Frame Size: " + std::to_string(memSize - 1) +
					"\n\tPage Faults: " + std::to_string(currPageFaults) + " @ Frame Size: " + std::to_string(memSize) + "\n";
				++numAnomalies;
			}
			prevPageFaults = currPageFaults;
			currPageFaults = 0;
		}
	}
	std::cout << anomalies << std::endl << std::endl;
	std::cout << "Anomaly detected " << numAnomalies << " times." << std::endl;

	return 0;
}

int getRand(int &low, int &high, std::mt19937 &mt)
{
	std::uniform_int_distribution<> dist(low, high);
	return dist(mt);
}

std::vector<int> generate(int &size, std::mt19937 &mt)
{
	int low = 0;
	int high = 250;
	std::vector<int> r(size);
	for (auto& e : r)
	{
		e = getRand(low, high, mt);
	}
	return r;
}

bool contains(std::unordered_map<int, bool> &hash, int &value)
{
	auto p = hash.find(value);
	if (p == hash.end()) return false;
	return true;
}

void clear(std::queue<int> &q)
{
	std::queue<int> empty;
	std::swap(q, empty);
}

void falsify(std::unordered_map<int, bool> &hash)
{
	for(auto value:hash){
		value.second = false;
	}
}
