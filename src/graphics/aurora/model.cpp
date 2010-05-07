/* eos - A reimplementation of BioWare's Aurora engine
 * Copyright (c) 2010 Sven Hesse (DrMcCoy), Matthew Hoops (clone2727)
 *
 * The Infinity, Aurora, Odyssey and Eclipse engines, Copyright (c) BioWare corp.
 * The Electron engine, Copyright (c) Obsidian Entertainment and BioWare corp.
 *
 * This file is part of eos and is distributed under the terms of
 * the GNU General Public Licence. See COPYING for more informations.
 */

/** @file graphics/aurora/model.cpp
 *  A 3D model of an object.
 */

#include "common/util.h"

#include "events/events.h"

#include "graphics/graphics.h"
#include "graphics/aurora/model.h"

namespace Graphics {

namespace Aurora {

Model::Node::Node() : parent(0), dangly(false), displacement(0), render(true) {
	position   [0] = 0;
	position   [1] = 0;
	position   [2] = 0;
	orientation[0] = 0;
	orientation[1] = 0;
	orientation[2] = 0;
	orientation[3] = 0;
}


Model::Model(ModelType type) : Renderable(GfxMan.getRenderableQueue((Graphics::RenderableQueue) type)),
                               _type(type), _superModel(0), _class(kClassOther), _scale(1.0) {

	_position[0] = 0.0;
	_position[1] = 0.0;
	_position[2] = 0.0;
}

Model::~Model() {
	for (std::list<Node *>::iterator node = _nodes.begin(); node != _nodes.end(); ++node) {
		if (*node) {
			TextureMan.release((*node)->texture);

			delete *node;
		}
	}
}

void Model::setPosition(float x, float y, float z) {
	_position[0] = x;
	_position[1] = y;
	_position[2] = z;
}

void Model::show() {
	addToQueue();
}

void Model::hide() {
	removeFromQueue();
}

void Model::newFrame() {
}

void Model::render() {
	if (_type == kModelTypeObject) {
		glTranslatef(0.0, -1.0, -3.0);

		float rotate = EventMan.getTimestamp() * 0.1;

		glRotatef(rotate, 0.0, 1.0, 0.0);
		glScalef(1.2, 1.2, 1.2);
	}

	if (_type == kModelTypeGUIFront) {
		glScalef(100, 100, 100);
		glTranslatef(-4.76, -1.37, 0.0);
	}

	glTranslatef(_position[0], _position[1], _position[2]);

	glColor4f(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (_type == kModelTypeObject)
		glRotatef(90.0, -1.0, 0.0, 0.0);

	for (std::list<Node *>::const_iterator node = _rootNodes.begin(); node != _rootNodes.end(); ++node) {
		glPushMatrix();
		glTranslatef((*node)->position[0], (*node)->position[1], (*node)->position[2]);
		glRotatef((*node)->orientation[3], (*node)->orientation[0], (*node)->orientation[1], (*node)->orientation[2]);
		renderNode(**node);
		glPopMatrix();
	}
}

void Model::renderNode(const Node &node) {
	if (node.render) {
		TextureMan.set(node.texture);

		glBegin(GL_TRIANGLES);

		for (FaceList::const_iterator face = node.faces.begin(); face != node.faces.end(); ++face) {
			glTexCoord2f(face->verticesTexture[0][0], face->verticesTexture[0][1]);
			glVertex3f(face->vertices[0][0], face->vertices[0][1], face->vertices[0][2]);
			glTexCoord2f(face->verticesTexture[1][0], face->verticesTexture[1][1]);
			glVertex3f(face->vertices[1][0], face->vertices[1][1], face->vertices[1][2]);
			glTexCoord2f(face->verticesTexture[2][0], face->verticesTexture[2][1]);
			glVertex3f(face->vertices[2][0], face->vertices[2][1], face->vertices[2][2]);
		}

		glEnd();
	}

	for (std::list<Node *>::const_iterator child = node.children.begin(); child != node.children.end(); ++child) {
		if (!*child)
			continue;

		glPushMatrix();
		glTranslatef((*child)->position[0], (*child)->position[1], (*child)->position[2]);
		glRotatef((*child)->orientation[3], (*child)->orientation[0], (*child)->orientation[1], (*child)->orientation[2]);
		renderNode(**child);
		glPopMatrix();
	}
}

} // End of namespace Aurora

} // End of namespace Graphics