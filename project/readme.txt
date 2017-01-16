

MIlling and screen drawing................................

Geometry.h
	bases of 2D Cartesian points using
	boost geometry.
	support for transformations, rotations, mirroring, etc...

object.h
	primitive objects for milling and drawing
	a base class: BaseItem and containers use SP_BaseItem
		shared pointers..

Drawing.h
	support for drawing objects.

notifying the document
https://www.codeproject.com/Articles/14706/Notifying-the-Document


DrawingObects
	wrapper for object_set_t called primatives

object_set_t a vector of ItemSet;

ItemSet
	owns asp_obj_vect_t
	which is a vector of shared pointers SP_BaseItem
