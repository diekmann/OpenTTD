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


/**
 * Get the direction pointing to the other end of the tunnel or bridge.
 * @param t The tile to analyze
 * @pre IsTileType(t, MP_TUNNELBRIDGE)
 * @return the direction pointing to the other end
 */
inline DiagDirection GetTunnelBridgeDirection(Tile t)
{
	assert(IsTileType(t, MP_TUNNELBRIDGE));
	return (DiagDirection)GB(t.m5(), 0, 2);
}

/**
 * Tunnel: Get the transport type of the tunnel (road or rail)
 * Bridge: Get the transport type of the bridge's ramp
 * @param t The tile to analyze
 * @pre IsTileType(t, MP_TUNNELBRIDGE)
 * @return the transport type in the tunnel/bridge
 */
inline TransportType GetTunnelBridgeTransportType(Tile t)
{
	assert(IsTileType(t, MP_TUNNELBRIDGE));
	return (TransportType)GB(t.m5(), 2, 2);
}

/**
 * Tunnel: Is this tunnel entrance in a snowy or desert area?
 * Bridge: Does the bridge ramp lie in a snow or desert area?
 * @param t The tile to analyze
 * @pre IsTileType(t, MP_TUNNELBRIDGE)
 * @return true if and only if the tile is in a snowy/desert area
 */
inline bool HasTunnelBridgeSnowOrDesert(Tile t)
{
	assert(IsTileType(t, MP_TUNNELBRIDGE));
	return HasBit(t.m7(), 5);
}

/**
 * Tunnel: Places this tunnel entrance in a snowy or desert area, or takes it out of there.
 * Bridge: Sets whether the bridge ramp lies in a snow or desert area.
 * @param t the tunnel entrance / bridge ramp tile
 * @param snow_or_desert is the entrance/ramp in snow or desert (true), when
 *                       not in snow and not in desert false
 * @pre IsTileType(t, MP_TUNNELBRIDGE)
 */
inline void SetTunnelBridgeSnowOrDesert(Tile t, bool snow_or_desert)
{
	assert(IsTileType(t, MP_TUNNELBRIDGE));
	SB(t.m7(), 5, 1, snow_or_desert);
}

/**
 * Determines type of the wormhole and returns its other end
 * @param t one end
 * @pre IsTileType(t, MP_TUNNELBRIDGE)
 * @return other end
 */
inline TileIndex GetOtherTunnelBridgeEnd(Tile t)
{
	assert(IsTileType(t, MP_TUNNELBRIDGE));
	return IsTunnel(t) ? GetOtherTunnelEnd(t) : GetOtherBridgeEnd(t);
}


/**
 * Get the reservation state of the rail tunnel/bridge
 * @pre IsTileType(t, MP_TUNNELBRIDGE) && GetTunnelBridgeTransportType(t) == TRANSPORT_RAIL
 * @param t the tile
 * @return reservation state
 */
inline bool HasTunnelBridgeReservation(Tile t)
{
	assert(IsTileType(t, MP_TUNNELBRIDGE));
	assert(GetTunnelBridgeTransportType(t) == TRANSPORT_RAIL);
	return HasBit(t.m5(), 4);
}

/**
 * Set the reservation state of the rail tunnel/bridge
 * @pre IsTileType(t, MP_TUNNELBRIDGE) && GetTunnelBridgeTransportType(t) == TRANSPORT_RAIL
 * @param t the tile
 * @param b the reservation state
 */
inline void SetTunnelBridgeReservation(Tile t, bool b)
{
	assert(IsTileType(t, MP_TUNNELBRIDGE));
	assert(GetTunnelBridgeTransportType(t) == TRANSPORT_RAIL);
	AssignBit(t.m5(), 4, b);
}

/**
 * Get the extended direction for tunnels (8 directions) when supported.
 * Falls back to standard direction for bridges and older tunnels.
 * @param t The tile to analyze
 * @pre IsTileType(t, MP_TUNNELBRIDGE)
 * @return the direction as full Direction (8 possible values)
 */
inline Direction GetTunnelBridgeFullDirection(Tile t)
{
	assert(IsTileType(t, MP_TUNNELBRIDGE));
	if (IsTunnel(t) && GetTunnelBridgeTransportType(t) == TRANSPORT_RAIL && HasBit(t.m5(), 5)) {
		// Rail tunnel with 8-direction support (bit 5 set)
		// Reconstruct direction from bits 0, 6-7
		uint8_t bit0 = GB(t.m5(), 0, 1);
		uint8_t bits67 = GB(t.m5(), 6, 2);
		uint8_t dir = bit0 | (bits67 << 1);
		return (Direction)dir;
	} else {
		// Standard 4-direction tunnel/bridge - convert DiagDirection to Direction
		return DiagDirToDir(GetTunnelBridgeDirection(t));
	}
}

/**
 * Get the reserved track bits for a rail tunnel/bridge
 * @pre IsTileType(t, MP_TUNNELBRIDGE) && GetTunnelBridgeTransportType(t) == TRANSPORT_RAIL
 * @param t the tile
 * @return reserved track bits
 */
inline TrackBits GetTunnelBridgeReservationTrackBits(Tile t)
{
	if (!HasTunnelBridgeReservation(t)) return TRACK_BIT_NONE;
	
	if (IsTunnel(t)) {
		// For tunnels, convert full direction to track bits
		Direction dir = GetTunnelBridgeFullDirection(t);
		switch (dir) {
			case DIR_N:  return TRACK_BIT_UPPER;
			case DIR_NE: return TRACK_BIT_Y;
			case DIR_E:  return TRACK_BIT_RIGHT;
			case DIR_SE: return TRACK_BIT_Y;
			case DIR_S:  return TRACK_BIT_LOWER;
			case DIR_SW: return TRACK_BIT_X;
			case DIR_W:  return TRACK_BIT_LEFT;
			case DIR_NW: return TRACK_BIT_X;
			default:     return TRACK_BIT_NONE;
		}
	} else {
		// For bridges, use the old logic
		return DiagDirToDiagTrackBits(GetTunnelBridgeDirection(t));
	}
}

#endif /* TUNNELBRIDGE_MAP_H */
