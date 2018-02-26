/*
*  Copyright(c) 2016 - 2017 Stanislaw Eppinger
*  Scripting based game called ChaiDwarfs
*
*  This file is part of ChaiDwarfs.
*
*  ChaiDwarfs is free software : you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.If not, see <http://www.gnu.org/licenses/>
*/
#include "ComponentSystem.hpp"
#include "src/rendering/SpriteRenderer.hpp"

using namespace cdwarfs::compSys;

MoveSprite_Sys::MoveSprite_Sys(
    const std::shared_ptr<EntityManager>& entManager,
    const std::shared_ptr<render::SpriteRenderer>& spriteRenderer
) :
    BaseVisitor(entManager),
    m_spriteRenderer(spriteRenderer)
{}

BaseVisitor::ReturnedCommands MoveSprite_Sys::operator()(const cmd::MoveSpriteLeft& cmd)
{
    m_spriteRenderer->spriteMove(cmd.dest, "moveLeft", cmd.row, cmd.col);
    return ReturnedCommands();
}

BaseVisitor::ReturnedCommands MoveSprite_Sys::operator()(const cmd::MoveSpriteRight& cmd)
{
    m_spriteRenderer->spriteMove(cmd.dest, "moveRight", cmd.row, cmd.col);
    return ReturnedCommands();
}

BaseVisitor::ReturnedCommands MoveSprite_Sys::operator()(const cmd::MoveSpriteUp& cmd)
{
    m_spriteRenderer->spriteMove(cmd.dest, "moveUp", cmd.row, cmd.col);
    return ReturnedCommands();
}

BaseVisitor::ReturnedCommands MoveSprite_Sys::operator()(const cmd::MoveSpriteDown& cmd)
{
    m_spriteRenderer->spriteMove(cmd.dest, "moveDown", cmd.row, cmd.col);
    return ReturnedCommands();
}
