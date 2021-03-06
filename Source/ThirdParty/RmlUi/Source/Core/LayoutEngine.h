/*
 * This source file is part of RmlUi, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://github.com/mikke89/RmlUi
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 * Copyright (c) 2019 The RmlUi Team, and contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef RMLUI_CORE_LAYOUTENGINE_H
#define RMLUI_CORE_LAYOUTENGINE_H

#include "LayoutBlockBox.h"

namespace Rml {

class Box;

/**
	@author Robert Curry
 */

class LayoutEngine
{
public:
	/// Formats the contents for a root-level element (usually a document, floating or replaced element). Establishes a new block formatting context.
	/// @param element[in] The element to lay out.
	/// @param containing_block[in] The size of the containing block.
	/// @param shrink_to_fit[in] True to shrink the element to the width of its contents.
	static bool FormatElement(Element* element, Vector2f containing_block);

	/// Positions a single element and its children within a block formatting context.
	/// @param[in] block_context_box The open block box to layout the element in.
	/// @param[in] element The element to lay out.
	static bool FormatElement(LayoutBlockBox* block_context_box, Element* element);

	static void* AllocateLayoutChunk(size_t size);
	static void DeallocateLayoutChunk(void* chunk);

private:
	/// Formats and positions an element as a block element.
	/// @param[in] block_context_box The open block box to layout the element in.
	/// @param[in] element The block element.
	static bool FormatElementBlock(LayoutBlockBox* block_context_box, Element* element);
	/// Formats and positions an element as an inline element.
	/// @param[in] block_context_box The open block box to layout the element in.
	/// @param[in] element The inline element.
	static bool FormatElementInline(LayoutBlockBox* block_context_box, Element* element);
	/// Positions an element as a sized inline element, formatting its internal hierarchy as a block element.
	/// @param[in] block_context_box The open block box to layout the element in.
	/// @param[in] element The inline-block element.
	static bool FormatElementInlineBlock(LayoutBlockBox* block_context_box, Element* element);
	/// Executes any special formatting for special elements.
	/// @param[in] block_context_box The open block box to layout the element in.
	/// @param[in] element The element to parse.
	/// @return True if the element was parsed as a special element, false otherwise.
	static bool FormatElementSpecial(LayoutBlockBox* block_context_box, Element* element);
};

} // namespace Rml
#endif
