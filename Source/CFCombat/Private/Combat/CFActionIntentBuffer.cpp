// Copyright Critical Failure Studio Ltd.

#include "Combat/CFActionIntentBuffer.h"

FCFActionIntentBuffer::FCFActionIntentBuffer()
	: bBufferOpen(false)
	, MaxIntentAge(0.15f)
{}

void FCFActionIntentBuffer::ClearBuffer()
{
	Entries.Empty();
}