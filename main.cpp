#include <iostream>
#include <queue>
#include <vector>
#include <random>
#include <unordered_set>
#include <string>

int getRand(int &, int &);
std::vector<int> generate(int &);
bool contains(std::unordered_set<int> &, int &);
void clear(std::queue<int> &q);

int main() {

	auto seqSize = 1000;
	std::vector<int> sequence;
	std::unordered_set<int> hash;
	std::queue<int> frames;
	int currPageFaults = 0;
	int prevPageFaults;
	unsigned int numSequences = 100;
	unsigned int maxFrames = 100;
	std::string anomalies= "";
	auto numAnomalies = 0;

	for (unsigned int seq = 1; seq <= numSequences; ++seq) {
		sequence = generate(seqSize);
		prevPageFaults = 1000;
		for (unsigned int memSize = 1; memSize <= maxFrames; ++memSize) {
			hash.clear();
			clear(frames);
			for (auto page : sequence) {
				auto test = contains(hash, page);
				if (!test && frames.size() >= memSize) {
					++currPageFaults;
					hash.erase(frames.front());
					frames.pop();
					hash.insert(page);
					frames.push(page);
				}
				else if (!test && frames.size() < memSize) {
					++currPageFaults;
					hash.insert(page);
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

	char dummy;
	std::cin >> dummy;

	return 0;
}

int getRand(int &low, int &high)
{
	static std::random_device rd;
	static std::mt19937 mt(rd());
	std::uniform_int_distribution<> dist(low, high);
	return dist(mt);
}

std::vector<int> generate(int &size)
{
	int low = 0;
	int high = 250;
	std::vector<int> r(size);
	for (auto& e : r)
	{
		e = getRand(low, high);
	}
	return r;
}

bool contains(std::unordered_set<int> &hash, int &value)
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