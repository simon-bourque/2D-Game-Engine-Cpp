
#include <Core/Core.h>
#include <2D/Scene/Tile.h>
#include <Core/Math/Geometry/Rectangle.h>

#include <pugixml.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "Utils.h"

using namespace std;

static void writeLevelBoundsChunk(ofstream& output, const pugi::xml_node& mapNode, float32& width);
static void writeTilesetsChunk(ofstream& output, const pugi::xml_node& mapNode, vector<pair<uint32, uint32>>& firstgids, uint32& numTilesets);
static void writeTilesChunk(ofstream& output, const pugi::xml_node& mapNode, vector<pair<uint32, uint32>>& firstgids, uint32 numTilesets, uint32 width);
static void writeCollidersChunk(ofstream& output, const pugi::xml_node& mapNode);

void convertLevel(const string& path) {
	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_file(path.c_str());

	if (!result) {
		cout << "Failed to load tmx file \'" << path << "\'" << endl;
		return;
	}

	string originalFileName = path.substr(path.find_last_of("/\\") + 1);
	string newFileName = originalFileName.substr(0, originalFileName.find_last_of(".")).append(".lvl");
	string newPath = path.substr(0, path.find_last_of("/\\") + 1).append(newFileName);

	ofstream output(newPath, std::ios_base::out | std::ios_base::binary);

	if (!output) {
		cout << "Failed to write new lvl file." << endl;
		output.close();
		return;
	}

	pugi::xml_node mapNode = doc.child("map");
	float32 width = 0;
	writeLevelBoundsChunk(output, mapNode, width);


	vector<pair<uint32, uint32>> firstgids;
	uint32 numTilesets = 0;
	writeTilesetsChunk(output, mapNode, firstgids, numTilesets);


	writeTilesChunk(output, mapNode, firstgids, numTilesets, width);

	writeCollidersChunk(output, mapNode);

	cout << "Done." << endl;

	output.close();
}

static void writeHeaderChunk() {

}

static void writeDependenciesChunk() {

}

static void writeLevelBoundsChunk(ofstream& output, const pugi::xml_node& mapNode, float32& width) {
	width = mapNode.attribute("width").as_float();
	float32 height = mapNode.attribute("height").as_float();
	float32 rectX = (width / 2.0f) - 0.5f;
	float32 rectY = (height / -2.0f) + 0.5f;

	writeFloat(output, rectX);
	writeFloat(output, rectY);
	writeFloat(output, width);
	writeFloat(output, height);
}

static void writeTilesetsChunk(ofstream& output, const pugi::xml_node& mapNode, vector<pair<uint32, uint32>>& firstgids, uint32& numTilesets) {
	auto tilesetNodes = mapNode.children("tileset");
	numTilesets = 0;
	for (const pugi::xml_node& tilesetNode : tilesetNodes) {
		numTilesets++;
	}

	writeUnsignedInt(output, numTilesets);

	for (const pugi::xml_node& tilesetNode : tilesetNodes) {
		pugi::xml_node imgNode = tilesetNode.child("image");
		string texturePath = imgNode.attribute("source").value();
		string textureName = texturePath.substr(texturePath.find_last_of("/\\") + 1);

		// Replace extension
		textureName = textureName.substr(0, textureName.find_last_of(".")) + ".tx";

		writeUnsignedInt(output, textureName.size());
		for (uint32 i = 0; i < textureName.size(); i++) {
			output.put(textureName[i]);
		}

		firstgids.push_back({ tilesetNode.attribute("firstgid").as_uint(), tilesetNode.attribute("tilecount").as_uint() });
	}
}

static void writeTilesChunk(ofstream& output, const pugi::xml_node& mapNode, vector<pair<uint32, uint32>>& firstgids, uint32 numTilesets, uint32 width) {
	auto layersNode = mapNode.children("layer");

	vector<pair<vector<Tile>, uint32>> finalLayers;

	for (const pugi::xml_node& layerNode : layersNode) {
		pugi::xml_node dataNode = layerNode.child("data");
		if (string(dataNode.attribute("encoding").value()) != "csv") {
			output.close();
			throw runtime_error("Failed to write new lvl file.  Encoding not supported.");
		}

		vector<Tile>* layers = new vector<Tile>[numTilesets];

		string data = dataNode.child_value();
		stringstream ss;
		float32 x = 0;
		float32 y = 0;

		auto parseData = [&ss, &firstgids, &layers, &x, &y, &width]() {
			int32 index = stoi(ss.str());
			uint32 tilesetIndex = 0;

			// If tile is not empty
			if (index > 0) {
				for (int32 i = 0; i < firstgids.size(); i++) {
					if (index >= firstgids[i].first && index < firstgids[i].second + firstgids[i].first) {
						tilesetIndex = i;
						index -= firstgids[i].first;
						break;
					}
				}
			}
			else {
				--index;
			}
			
			// If tile is not empty
			if (index >= 0) {
				Tile tile(x, -y, index);
				layers[tilesetIndex].push_back(tile);
			}
			x++;
			if (x >= width) {
				x = 0;
				y++;
			}
			ss.str("");
		};

		for (int32 i = 0; i < data.size(); i++) {
			if (isdigit(data[i])) {
				ss << data[i];
			}
			else if (data[i] == ',') {
				parseData();
			}
		}
		parseData();

		for (int32 i = 0; i < numTilesets; i++) {
			if (!layers[i].empty()) {
				finalLayers.push_back({layers[i], i});
			}
		}

		delete[] layers;
	}

	writeUnsignedInt(output, finalLayers.size());
	for (auto& layer : finalLayers) {
		writeUnsignedInt(output, layer.first.size());
		writeUnsignedInt(output, layer.second);

		for (const Tile& tile : layer.first) {
			writeFloat(output, tile.x);
			writeFloat(output, tile.y);
			writeInt(output, tile.index);
		}
	}
}

static void writeCollidersChunk(ofstream& output, const pugi::xml_node& mapNode) {
	vector<geo::Rectangle> colliders;
	pugi::xml_node groupNode = mapNode.child("objectgroup");

	if (string(groupNode.attribute("name").value()) == "collision") {
		for (auto it = groupNode.begin(); it != groupNode.end(); it++) {
			float32 collX = it->attribute("x").as_int() / ((float32)mapNode.attribute("tilewidth").as_int());
			float32 collY = it->attribute("y").as_int() / ((float32)mapNode.attribute("tileheight").as_int());
			float32 collWidth = it->attribute("width").as_int() / ((float32)mapNode.attribute("tilewidth").as_int());
			float32 collHeight = it->attribute("height").as_int() / ((float32)mapNode.attribute("tileheight").as_int());

			// Shift origin to center
			collX += (collWidth / 2.0f) - 0.5f;
			collY += (collHeight / 2.0f) - 0.5f;

			geo::Rectangle collider(collX, collY, collWidth, collHeight);
			colliders.push_back(collider);
		}
	}

	writeUnsignedInt(output, colliders.size());

	for (const geo::Rectangle& collider : colliders) {
		writeFloat(output, collider.getX());
		writeFloat(output, -collider.getY());
		writeFloat(output, collider.getWidth());
		writeFloat(output, collider.getHeight());
	}
}