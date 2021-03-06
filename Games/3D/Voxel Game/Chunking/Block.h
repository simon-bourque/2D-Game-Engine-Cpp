#pragma once
//External includes
#include "glm\glm.hpp"

//Local headers
#include "Core/Types.h"

enum class BlockType
{
	GRASS, LOG, LEAVES, DIRT, SAND, SNOW
};

struct Block
{
	Block(){}
	Block(glm::vec3 position, BlockType blockType)
		: m_position(position)
		, m_blockType(blockType)
	{}

	//Getters
	glm::vec3 getPosition() const { return m_position; }
	BlockType getBlockType() const { return m_blockType; }

private:
	//Local position of the block in its chunk
	glm::vec3 m_position;

	//Type of the block
	BlockType m_blockType;
};