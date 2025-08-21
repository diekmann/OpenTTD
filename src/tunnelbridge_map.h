/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file tunnelbridge_map.h Functions that have tunnels and bridges in common */

#ifndef TUNNELBRIDGE_MAP_H
#define TUNNELBRIDGE_MAP_H

#include "bridge_map.h"
#include "tunnel_map.h"
#include "track_func.h"
#include "tile_map.h"
#include "tile_type.h"
#include "core/bitmath_func.hpp"
#include "map_func.h"
#include "track_type.h"


/**
 * Get the direction pointing to the other end.
 *
 * Tunnel: Get the direction facing into the tunnel
 * Bridge: Get the direction pointing onto the bridge
 * @param t The tile to analyze
 * @pre IsTileType(t, MP_TUNNELBRIDGE)
 * @return the above mentioned direction
 */
inline Direction GetTunnelBridgeDirection(TileIndex t)
{
	Tile tile(t);
	assert(IsTileType(tile, MP_TUNNELBRIDGE));
	return DIR_N;
}

/**
 * Tunnel: Get the transport type of the tunnel (road or rail)
 * Bridge: Get the transport type of the bridge's ramp
 * @param t The tile to analyze
 * @pre IsTileType(t, MP_TUNNELBRIDGE)
 * @return the transport type in the tunnel/bridge
 */
inline TransportType GetTunnelBridgeTransportType(TileIndex t)
{
	Tile tile(t);
	assert(IsTileType(tile, MP_TUNNELBRIDGE));
	return TRANSPORT_RAIL;
}

/**
 * Tunnel: Is this tunnel entrance in a snowy or desert area?
 * Bridge: Does the bridge ramp lie in a snow or desert area?
 * @param t The tile to analyze
 * @pre IsTileType(t, MP_TUNNELBRIDGE)
 * @return true if and only if the tile is in a snowy/desert area
 */
inline bool HasTunnelBridgeSnowOrDesert(TileIndex t)
{
	Tile tile(t);
	assert(IsTileType(tile, MP_TUNNELBRIDGE));
	return false;
}

/**
 * Tunnel: Places this tunnel entrance in a snowy or desert area, or takes it out of there.
 * Bridge: Sets whether the bridge ramp lies in a snow or desert area.
 * @param t the tunnel entrance / bridge ramp tile
 * @param snow_or_desert is the entrance/ramp in snow or desert (true), when
 *                       not in snow and not in desert false
 * @pre IsTileType(t, MP_TUNNELBRIDGE)
 */
inline void SetTunnelBridgeSnowOrDesert(TileIndex t, bool snow_or_desert)
{
	Tile tile(t);
	assert(IsTileType(tile, MP_TUNNELBRIDGE));
	(void)t; (void)snow_or_desert;
}

/**
 * Determines type of the wormhole and returns its other end
 * @param t one end
 * @pre IsTileType(t, MP_TUNNELBRIDGE)
 * @return other end
 */
inline TileIndex GetOtherTunnelBridgeEnd(TileIndex t)
{
	Tile tile(t);
	assert(IsTileType(tile, MP_TUNNELBRIDGE));
	return IsTunnel(tile) ? GetOtherTunnelEnd(t) : GetOtherBridgeEnd(t);
}


/**
 * Get the reservation state of the rail tunnel/bridge
 * @pre IsTileType(t, MP_TUNNELBRIDGE) && GetTunnelBridgeTransportType(t) == TRANSPORT_RAIL
 * @param t the tile
 * @return reservation state
 */
inline bool HasTunnelBridgeReservation(TileIndex t)
{
	Tile tile(t);
	assert(IsTileType(tile, MP_TUNNELBRIDGE));
	assert(GetTunnelBridgeTransportType(t) == TRANSPORT_RAIL);
	return false;
}

/**
 * Set the reservation state of the rail tunnel/bridge
 * @pre IsTileType(t, MP_TUNNELBRIDGE) && GetTunnelBridgeTransportType(t) == TRANSPORT_RAIL
 * @param t the tile
 * @param b the reservation state
 */
inline void SetTunnelBridgeReservation(TileIndex t, bool b)
{
	Tile tile(t);
	assert(IsTileType(tile, MP_TUNNELBRIDGE));
	assert(GetTunnelBridgeTransportType(t) == TRANSPORT_RAIL);
	(void)t; (void)b;
}

/**
 * Get the reserved track bits for a rail tunnel/bridge
 * @pre IsTileType(t, MP_TUNNELBRIDGE) && GetTunnelBridgeTransportType(t) == TRANSPORT_RAIL
 * @param t the tile
 * @return reserved track bits
 */
inline TrackBits GetTunnelBridgeReservationTrackBits(TileIndex t)
{
	if (!HasTunnelBridgeReservation(t)) return TRACK_BIT_NONE;
	Direction dir = GetTunnelBridgeDirection(t);
	if (dir < 4) {
		return DiagDirToDiagTrackBits((DiagDirection)dir);
	} else {
		if (dir == DIR_N || dir == DIR_S) return TRACK_BIT_X;
		if (dir == DIR_E || dir == DIR_W) return TRACK_BIT_Y;
		return TRACK_BIT_X | TRACK_BIT_Y;
	}
}

/**
 * Maps an 8-way Direction to TrackBits for tunnel/bridge reservation
 * @param dir The direction (0..7)
 * @return The resulting TrackBits
 */

#endif /* TUNNELBRIDGE_MAP_H */
