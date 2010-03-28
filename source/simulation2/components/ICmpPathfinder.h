/* Copyright (C) 2010 Wildfire Games.
 * This file is part of 0 A.D.
 *
 * 0 A.D. is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * 0 A.D. is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 0 A.D.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INCLUDED_ICMPPATHFINDER
#define INCLUDED_ICMPPATHFINDER

#include "simulation2/system/Interface.h"

#include "simulation2/helpers/Position.h"

#include <vector>

/**
 * Pathfinder algorithm.
 *
 * The pathfinder itself does not depend on other components. Instead, it contains an abstract
 * view of the game world, based a series of collision shapes (circles and squares), which is
 * updated by calls from other components (typically CCmpObstruction).
 *
 * Internally it quantises the shapes onto a grid and computes paths over the grid, but the interface
 * does not expose that detail.
 */
class ICmpPathfinder : public IComponent
{
public:
	struct Goal
	{
		entity_pos_t x, z;
		entity_pos_t minRadius, maxRadius;
	};

	/**
	 * Returned paths are currently represented as a series of waypoints.
	 * These happen to correspond to the centers of horizontally/vertically adjacent tiles
	 * along the path, but it's probably best not to rely on that.
	 */
	struct Waypoint
	{
		entity_pos_t x, z;
		u32 cost; // currently a meaningless number
	};

	/**
	 * Returned path.
	 * Waypoints are in *reverse* order (the earliest is at the back of the list)
	 */
	struct Path
	{
		std::vector<Waypoint> m_Waypoints;
	};

	/**
	 * Determine whether a unit (of radius r) can move between the given points in a straight line,
	 * without hitting any obstacles.
	 * This is based on the exact list of obtruction shapes, not the grid approximation.
	 * This should be used as a shortcut to avoid using the pathfinding algorithm in simple cases,
	 * and for more refined movement along the found paths.
	 */
	virtual bool CanMoveStraight(entity_pos_t x0, entity_pos_t z0, entity_pos_t x1, entity_pos_t z1, entity_pos_t r, u32& cost) = 0;

	/**
	 * Compute a path between the given points, and return the set of waypoints.
	 */
	virtual void ComputePath(entity_pos_t x0, entity_pos_t z0, const Goal& goal, Path& ret) = 0;

	/**
	 * Compute a path between the given points, and draw the latest such path as a terrain overlay.
	 */
	virtual void SetDebugPath(entity_pos_t x0, entity_pos_t z0, const Goal& goal) = 0;

	/**
	 * Toggle the storage and rendering of debug info.
	 */
	virtual void SetDebugOverlay(bool enabled) = 0;

	DECLARE_INTERFACE_TYPE(Pathfinder)
};

#endif // INCLUDED_ICMPPATHFINDER
