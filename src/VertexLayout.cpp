#include "pch.h"
#include "VertexLayout.h"


void VertexLayout::Enable() const
{
	unsigned offset = 0;
	for (unsigned i = 0; i < attribs.size(); ++i)
	{
		const auto& at = attribs[i];
		glVertexAttribPointer(i, at.count, at.type, at.normalized, stride, (void*)offset);
		glEnableVertexAttribArray(i);
		offset += at.count * at.GetTypeSize();
	}
}
