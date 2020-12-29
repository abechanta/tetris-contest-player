//
// Copyright (c) 2010, Abe Chanta
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// - Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// - Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#ifndef TCEVENT_H
#define TCEVENT_H
//! @file

#include "tctype.h"

//----------------------------------------------------------------------------
typedef uint32_t		TCEVENT;	//!< ゲームプレイ中に発生した状況を伝達するための型です。
enum _TCEVENT
{
	// 入力イベント
	TCEVENT_MOVE_L		= (1U << 0),	//!< ピースを左に移動
	TCEVENT_MOVE_R		= (1U << 1),	//!< ピースを右に移動
	TCEVENT_MOVE		= (TCEVENT_MOVE_L | TCEVENT_MOVE_R),
	TCEVENT_ROT_L		= (1U << 2),	//!< ピースを左回転
	TCEVENT_ROT_R		= (1U << 3),	//!< ピースを右に回転
	TCEVENT_ROT			= (TCEVENT_ROT_L | TCEVENT_ROT_R),

	// 入力フィードバックイベント
	TCEVENT_nMOVE_L		= (1U << 8),	//!< ピースを左に移動させたかったが壁が邪魔で移動できなかった
	TCEVENT_nMOVE_R		= (1U << 9),	//!< ピースを右に移動させたかったが壁が邪魔で移動できなかった
	TCEVENT_nROT_L		= (1U << 10),	//!< ピースを左回転させたかったが壁が邪魔で移動できなかった
	TCEVENT_nROT_R		= (1U << 11),	//!< ピースを右回転させたかったが壁が邪魔で移動できなかった

	// シーケンスイベント
	TCEVENT_PREP		= (1U << 12),	//!< 次のフレームにピースが落下開始する瞬間
	TCEVENT_FALL		= (1U << 13),	//!< ピースが落下中
	TCEVENT_LAND		= (1U << 14),	//!< ピースが接地している状態
	TCEVENT_PENALTY		= (1U << 15),	//!< 前回の入力がペナルティ対象となった

	TCEVENT_ROUND_OVER	= (1U << 29),	//!< ラウンドオーバー
	TCEVENT_OPPONENT	= (1U << 30),	//!< 相手のフィールドに対してピースを配置している最中（対戦時のみ）
	TCEVENT_GAME_OVER	= (1U << 31),	//!< ゲームオーバー
	TCEVENT_MASK_PRE	= (0xFFFFFF00U & ~TCEVENT_PREP & ~TCEVENT_FALL & ~TCEVENT_LAND),
};



//----------------------------------------------------------------------------
#endif	// TCEVENT_H
