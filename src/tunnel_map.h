/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file tunnel_map.h Map accessors for tunnels. */

// Ensure full definition of Tile and helpers are available for inline functions
#include "map_func.h"
#include "tile_map.h"
#ifndef TUNNEL_MAP_H
#define TUNNEL_MAP_H
#include "rail_map.h"
#include "road_map.h"
#include "rail_type.h"
#include "road_type.h"
#include "tile_type.h"
#include "core/bitmath_func.hpp"
#include <cassert>


/**
 * Is this a tunnel (entrance)?
 * @param t the tile that might be a tunnel
 * @pre IsTileType(t, MP_TUNNELBRIDGE)
 * @return true if and only if this tile is a tunnel (entrance)
 */
inline bool IsTunnel(TileIndex tile)
{
	Tile t(tile);
	assert(IsTileType(t, MP_TUNNELBRIDGE));
	return !HasBit(t.m5(), 7);
}

/**
 * Is this a tunnel (entrance)?
 * @param t the tile that might be a tunnel
 * @return true if and only if this tile is a tunnel (entrance)
 */
inline bool IsTunnelTile(TileIndex tile)
{
	Tile t(tile);
	bool is_tunnel = IsTileType(t, MP_TUNNELBRIDGE) && IsTunnel(tile);
	if (is_tunnel) {
		/* Assert that the high bit of m2 is always 0 for tunnel tiles */
		assert((t.m2() & 0x80) == 0);
	}
	return is_tunnel;
}

TileIndex GetOtherTunnelEnd(TileIndex);
bool IsTunnelInWay(TileIndex, int z);
bool IsTunnelInWayDir(TileIndex tile, int z, DiagDirection dir);

/**
 * Makes a road tunnel entrance
 * @param t the entrance of the tunnel
 * @param o the owner of the entrance
 * @param d the direction facing out of the tunnel
 * @param r the road type used in the tunnel
 */
inline void MakeRoadTunnel(Tile t, Owner o, Direction d, RoadType road_rt, RoadType tram_rt)
{
	SetTileType(t, MP_TUNNELBRIDGE);
	SetTileOwner(t, o);
	t.m2() = 0;
	t.m3() = 0;
	t.m4() = 0;
	// Encode direction: normal (0..3) in m5 bits 1..0, diagonal (4..7) in m5 bits 1..0 + m2 bit 7
	if (d < 4) {
		t.m5() = (TRANSPORT_ROAD << 2) | d;
		t.m2() &= 0x7F; // clear high bit
	} else {
		t.m5() = (TRANSPORT_ROAD << 2) | (d - 4);
		t.m2() |= 0x80; // set high bit
	}
	SB(t.m6(), 2, 4, 0);
	t.m7() = 0;
	t.m8() = 0;
	SetRoadOwner(t, RTT_ROAD, o);
	if (o != OWNER_TOWN) SetRoadOwner(t, RTT_TRAM, o);
	SetRoadTypes(t, road_rt, tram_rt);
}

/**
 * Makes a rail tunnel entrance
 * @param t the entrance of the tunnel
 * @param o the owner of the entrance
 * @param d the direction facing out of the tunnel (Direction, 0..7)
 * @param r the rail type used in the tunnel
 */
inline void MakeRailTunnel(Tile t, Owner o, Direction d, RailType r)
{
	SetTileType(t, MP_TUNNELBRIDGE);
	SetTileOwner(t, o);
	t.m2() = 0;
	t.m3() = 0;
	t.m4() = 0;
	// Encode direction: normal (0..3) in m5 bits 1..0, diagonal (4..7) in m5 bits 1..0 + m2 bit 7
	if (d < 4) {
		t.m5() = (TRANSPORT_RAIL << 2) | d;
		t.m2() &= 0x7F; // clear high bit
	} else {
		t.m5() = (TRANSPORT_RAIL << 2) | (d - 4);
		t.m2() |= 0x80; // set high bit
	}
	SB(t.m6(), 2, 4, 0);
	t.m7() = 0;
	t.m8() = 0;
	SetRailType(t, r);
}

/**
 * Get the direction of a tunnel (now supports 8 directions).
 * @param t the tile to get the direction from
 * @return Direction (0..7)
 */
// ...existing code...

/**
 * Set the direction of a tunnel (now supports 8 directions).
 * @param t the tile to set the direction for
 * @param dir Direction (0..7)
 */
	/**
	 * Get the direction of a tunnel (supports 8 directions).
	 * @param t the tile to get the direction from
	 * @return Direction (0..7)
	 */
	/**
	 * Set the direction of a tunnel (supports 8 directions).
inline Direction GetTunnelDirection(TileIndex tile)
{
	Tile t(tile);
	assert(IsTileType(t, MP_TUNNELBRIDGE));
	uint8_t dir = t.m5() & 0x03;
	if (t.m2() & 0x80) dir += 4;
	return (Direction)dir;
}
	 * @param t the tile to set the direction for
	 * @param dir Direction (0..7)
	 */
// ...existing code...

#endif /* TUNNEL_MAP_H */
