//-----------------------------------------------------------------------------
// File:			W_VertexBuffer.cpp
// Original Author:	Gordon Wood
//
// See header for notes
//-----------------------------------------------------------------------------
#include "W_VertexBuffer.h"

#include <iostream>
namespace wolf
{
//----------------------------------------------------------
// Constructor
//----------------------------------------------------------
VertexBuffer::VertexBuffer(unsigned int p_uiLength) : m_uiLength(p_uiLength)
{
	glGenBuffers(1, &m_uiBuffer);
}

//----------------------------------------------------------
// Constructor
//----------------------------------------------------------
VertexBuffer::VertexBuffer(const void* p_pData, unsigned int p_uiLength) : m_uiLength(p_uiLength)
{
	glGenBuffers(1, &m_uiBuffer);
	Write(p_pData);
}

//----------------------------------------------------------
// Destructor
//----------------------------------------------------------
VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1,&m_uiBuffer);
}

//----------------------------------------------------------
// Fills this vertex buffer with the given data
//----------------------------------------------------------
/*
void VertexBuffer::Write(const void* p_pData, int p_iLength)
{
	Bind();
	//glBufferData(GL_ARRAY_BUFFER, p_iLength == -1 ? m_uiLength : p_iLength, p_pData, GL_DYNAMIC_DRAW);	// Used to be static

	glBufferData(GL_ARRAY_BUFFER, p_iLength, nullptr, GL_DYNAMIC_DRAW); // Orphan the buffer
    glBufferData(GL_ARRAY_BUFFER, p_iLength, p_pData, GL_DYNAMIC_DRAW); // Upload new data
}
*/
void VertexBuffer::Write(const void* p_pData, int p_iLength) {
    Bind();
	glBufferData(GL_ARRAY_BUFFER, p_iLength == -1 ? m_uiLength : p_iLength, p_pData, GL_STATIC_DRAW);
}

void VertexBuffer::Update(const void* p_pData, int p_iLength){
	Bind();

    // Determine the buffer size
    int bufferSize = (p_iLength == -1) ? p_iLength : p_iLength;

	// Orphan the buffer
    glBufferData(GL_ARRAY_BUFFER, bufferSize, nullptr, GL_DYNAMIC_DRAW);

	// Upload the new data
	glBufferData(GL_ARRAY_BUFFER, bufferSize, p_pData, GL_DYNAMIC_DRAW);
}
//----------------------------------------------------------
// Binds this buffer
//----------------------------------------------------------
void VertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
}

}
