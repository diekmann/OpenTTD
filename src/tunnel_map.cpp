/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file tunnel_map.cpp Map accessors for tunnels. */

#include "stdafx.h"
#include "tunnelbridge_map.h"
#include "debug.h"

#include "safeguards.h"


/**
 * Gets the other end of the tunnel. Where a vehicle would reappear when it
 * enters at the given tile.
 * @param tile the tile to search from.
 * @return the tile of the other end of the tunnel.
 */
TileIndex GetOtherTunnelEnd(TileIndex tile)
{
	assert(IsTunnelTile(tile));
	
	Direction dir = GetTunnelBridgeFullDirection(tile);
	TileIndexDiff delta = TileOffsByDir(dir);
	int z = GetTileZ(tile);
	Direction reverse_dir = ReverseDir(dir);

	TileIndex current_tile = tile;
	int steps = 0;
	const int MAX_TUNNEL_STEPS = 1000; // Safety limit
	
	// Add comprehensive debug logging
	Debug(misc, 0, "GetOtherTunnelEnd: Starting search from tile {} (x={},y={}), dir={}, reverse_dir={}, z={}", 
		  tile.base(), TileX(tile), TileY(tile), (uint)dir, (uint)reverse_dir, z);
	Debug(misc, 0, "GetOtherTunnelEnd: Direction offsets - dir={} gives delta={}", (uint)dir, delta);
	
	do {
		current_tile += delta;
		steps++;
		
		if (steps <= 10) {
			Debug(misc, 0, "GetOtherTunnelEnd: Step {} - tile {} (x={},y={}), is_tunnel={}, z={}", 
				  steps, current_tile.base(), TileX(current_tile), TileY(current_tile), 
				  IsTunnelTile(current_tile) ? 1 : 0, GetTileZ(current_tile));
			if (IsTunnelTile(current_tile)) {
				Debug(misc, 0, "GetOtherTunnelEnd: Step {} - tunnel has dir={} (we want {})", 
					  steps, (uint)GetTunnelBridgeFullDirection(current_tile), (uint)reverse_dir);
			}
		}
		
		// Safety check to prevent infinite loops or going off map
		if (!IsValidTile(current_tile) || steps > MAX_TUNNEL_STEPS) {
			Debug(misc, 0, "GetOtherTunnelEnd: FAILED - start_tile={}, dir={}, reverse_dir={}, current_tile={}, steps={}, valid={}", 
				  tile.base(), (uint)dir, (uint)reverse_dir, current_tile.base(), steps, IsValidTile(current_tile) ? 1 : 0);
			if (IsValidTile(current_tile)) {
				Debug(misc, 0, "GetOtherTunnelEnd: Final tile {} (x={},y={}): is_tunnel={}, has_dir={}, z={} (expected z={})", 
					  current_tile.base(), TileX(current_tile), TileY(current_tile),
					  IsTunnelTile(current_tile) ? 1 : 0,
					  IsTunnelTile(current_tile) ? (uint)GetTunnelBridgeFullDirection(current_tile) : 999,
					  GetTileZ(current_tile), z);
			}
			NOT_REACHED();
		}
	} while (
		!IsTunnelTile(current_tile) ||
		GetTunnelBridgeFullDirection(current_tile) != reverse_dir ||
		GetTileZ(current_tile) != z
	);

	Debug(misc, 0, "GetOtherTunnelEnd: SUCCESS - found end at tile {} after {} steps", current_tile.base(), steps);
	return current_tile;
}


/**
 * Is there a tunnel in the way in the given direction?
 * @param tile the tile to search from.
 * @param z    the 'z' to search on.
 * @param dir  the direction to start searching to.
 * @return true if and only if there is a tunnel.
 */
bool IsTunnelInWayDir(TileIndex tile, int z, DiagDirection dir)
{
	TileIndexDiff delta = TileOffsByDiagDir(dir);
	int height;

	do {
		tile -= delta;
		if (!IsValidTile(tile)) return false;
		height = GetTileZ(tile);
	} while (z < height);

	return z == height && IsTunnelTile(tile) && GetTunnelBridgeDirection(tile) == dir;
}

/**
 * Is there a tunnel in the way in any direction?
 * @param tile the tile to search from.
 * @param z the 'z' to search on.
 * @return true if and only if there is a tunnel.
 */
bool IsTunnelInWay(TileIndex tile, int z)
{
	return IsTunnelInWayDir(tile, z, (TileX(tile) > (Map::MaxX() / 2)) ? DIAGDIR_NE : DIAGDIR_SW) ||
			IsTunnelInWayDir(tile, z, (TileY(tile) > (Map::MaxY() / 2)) ? DIAGDIR_NW : DIAGDIR_SE);
}
