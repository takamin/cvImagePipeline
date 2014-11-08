// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once

#if defined(_MSC_VER)
#include "targetver.h"
#endif

#include <stdio.h>
#if defined(_MSC_VER)
#include <tchar.h>
#endif
#include <string>
#include <sstream>
#include "cvImagePipeline.h"

using namespace std;
using namespace cvImagePipeline;
using namespace cvImagePipeline::Filter;

