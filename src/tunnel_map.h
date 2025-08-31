/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file tunnel_map.h Map accessors for tunnels. */

#ifndef TUNNEL_MAP_H
#define TUNNEL_MAP_H

#include "rail_map.h"
#include "road_map.h"


/**
 * Is this a tunnel (entrance)?
 * @param t the tile that might be a tunnel
 * @pre IsTileType(t, MP_TUNNELBRIDGE)
 * @return true if and only if this tile is a tunnel (entrance)
 */
inline bool IsTunnel(Tile t)
{
	assert(IsTileType(t, MP_TUNNELBRIDGE));
	return !HasBit(t.m5(), 7);
}

/**
 * Is this a tunnel (entrance)?
 * @param t the tile that might be a tunnel
 * @return true if and only if this tile is a tunnel (entrance)
 */
inline bool IsTunnelTile(Tile t)
{
	return IsTileType(t, MP_TUNNELBRIDGE) && IsTunnel(t);
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
inline void MakeRoadTunnel(Tile t, Owner o, DiagDirection d, RoadType road_rt, RoadType tram_rt)
{
	SetTileType(t, MP_TUNNELBRIDGE);
	SetTileOwner(t, o);
	t.m2() = 0;
	t.m3() = 0;
	t.m4() = 0;
	t.m5() = 0;  // Clear all bits first
	// For road tunnels, continue to use only 2 bits (DiagDirection only)
	SB(t.m5(), 0, 2, d);           // Store direction in bits 0-1 only
	SB(t.m5(), 2, 2, TRANSPORT_ROAD);
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
 * @param d the direction facing out of the tunnel (supports all 8 directions)
 * @param r the rail type used in the tunnel
 */
inline void MakeRailTunnel(Tile t, Owner o, Direction d, RailType r)
{
	SetTileType(t, MP_TUNNELBRIDGE);
	SetTileOwner(t, o);
	t.m2() = 0;
	t.m3() = 0;
	t.m4() = 0;
	t.m5() = 0;  // Clear all bits first
	
	// For 8-direction tunnels, use a different storage strategy:
	// If it's an orthogonal direction, use extended format with bit 5 as flag
	// If it's a diagonal direction, use standard DiagDirection format
	if (d == DIR_N || d == DIR_E || d == DIR_S || d == DIR_W) {
		// Orthogonal directions - use extended format
		// Store direction in bits 0-2, but we need to avoid conflict with transport type in bits 2-3
		// Solution: Use bits 6-7 and bit 0 to store the 3-bit direction
		SB(t.m5(), 0, 1, d & 1);           // Store bit 0 of direction
		SB(t.m5(), 6, 2, (d >> 1) & 3);   // Store bits 1-2 of direction  
		SB(t.m5(), 5, 1, 1);              // Set bit 5 to indicate extended format
	} else {
		// Diagonal directions - use standard format  
		DiagDirection diag_d = DirToDiagDir(d);
		SB(t.m5(), 0, 2, diag_d);          // Store DiagDirection in bits 0-1
		SB(t.m5(), 5, 1, 0);              // Clear bit 5 to indicate standard format
	}
	
	SB(t.m5(), 2, 2, TRANSPORT_RAIL);
	SB(t.m6(), 2, 4, 0);
	t.m7() = 0;
	t.m8() = 0;
	SetRailType(t, r);
}

/**
 * Makes a rail tunnel entrance (backward compatibility with DiagDirection)
 * @param t the entrance of the tunnel
 * @param o the owner of the entrance
 * @param d the direction facing out of the tunnel
 * @param r the rail type used in the tunnel
 */
inline void MakeRailTunnel(Tile t, Owner o, DiagDirection d, RailType r)
{
	SetTileType(t, MP_TUNNELBRIDGE);
	SetTileOwner(t, o);
	t.m2() = 0;
	t.m3() = 0;
	t.m4() = 0;
	// For backward compatibility, use standard 4-direction format
	t.m5() = 0;  // Clear all bits first
	SB(t.m5(), 0, 2, d);           // Store direction in bits 0-1 only
	SB(t.m5(), 2, 2, TRANSPORT_RAIL);
	SB(t.m6(), 2, 4, 0);
	t.m7() = 0;
	t.m8() = 0;
	SetRailType(t, r);
}

#endif /* TUNNEL_MAP_H */
