/*****************************************************************************
The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org.
******************************************************************************
The MIT License (MIT)

Copyright(c) 2015 Conrad Rozetti

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*****************************************************************************/

#pragma once

#define _6UG_METHOD(R, NAME) NAMESPACED_METHOD(debugger, 6UG, R, NAME) 
#define _6UG_METHODX(R, NAME, P1) NAMESPACED_METHODX(debugger, 6UG, R, NAME, P1)
#define _6UG_METHODXX(R, NAME, P1, P2) NAMESPACED_METHODXX(debugger, 6UG, R, NAME, P1, P2)

#define _6UG_METHOD_NAME(NAME) (_6UG##_##NAME)
