#define QT_NO_EMIT

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpyDetectorTwig.h"
#include "Iguana/QtGUI/interface/ISpyReadService.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <QtGui>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

ISpyDetectorTwig::ISpyDetectorTwig (IgState *state, IgTwig *parent,
				    const std::string &name /* = "" */)
    : ISpyQueuedTwig (state, parent, name)
{}

void
ISpyDetectorTwig::onNewEvent (ISpyEventMessage& message)
{
    ISpyQueuedTwig::onNewEvent (message);

    if (ISpyReadService *readService = ISpyReadService::get (state ()))
    {
	IgDataStorage *storage = readService->esStorage ();
	if (!storage->empty () && storage->hasCollection ("Geometry_V1"))
	{	    
	    IgCollection &geometry = storage->getCollection ("Geometry_V1");
	    if (geometry.size () > 0)
	    {
		IgColumnHandle detid = geometry.getHandleByLabel ("detid");
		IgColumnHandle name = geometry.getHandleByLabel ("shape");
		IgColumnHandle f1 = geometry.getHandleByLabel ("front_1");
		IgColumnHandle f2 = geometry.getHandleByLabel ("front_2");
		IgColumnHandle f3 = geometry.getHandleByLabel ("front_3");
		IgColumnHandle f4 = geometry.getHandleByLabel ("front_4");
		IgColumnHandle b1 = geometry.getHandleByLabel ("back_1");
		IgColumnHandle b2 = geometry.getHandleByLabel ("back_2");
		IgColumnHandle b3 = geometry.getHandleByLabel ("back_3");
		IgColumnHandle b4 = geometry.getHandleByLabel ("back_4");

		SoSeparator *sep = dynamic_cast<SoSeparator *>(ISpyQueuedTwig::rep ());

		IgCollection &driftTubes = storage->getCollection ("DTs_V1");
		IgAssociationSet &dtGeometry = storage->getAssociationSet ("DTGeometry_V1");

		SoVertexProperty *vertices = new SoVertexProperty;

		int i = 0;
		std::vector<int> lineIndices;
		std::vector<SbVec3f> corners;		

		IgCollectionIterator cit = driftTubes.begin ();
		IgCollectionIterator cend = driftTubes.end ();
		for (; cit != cend; cit++) 
		{
		    IgCollectionItem chamber = *cit;
		    qDebug() << "DT detid " << chamber.get<int>("detid");
			
		    IgAssociatedSet view = storage->getAssociatedSet("DTGeometry_V1", chamber, LEFT_ASSOCIATED);

		    // 			for (IgAssociatedSet::Iterator a = view.begin();
		    // 			     a != view.end();
		    // 			     a++)
		    // 			{
		    //                           IgCollectionItem m = *a;
		    for (IgAssociationSet::Iterator a = dtGeometry.begin();
			 a != dtGeometry.end();
			 a++)
		    {
			if(a->first().objectId() == chamber.currentRow())
			{
			    IgCollectionItem m(&geometry, a->second().objectId());
			    qDebug() << " - matched DT detid " << m.get<int>("detid");

			    IgV3d p1  = m.get<IgV3d> ("front_1");

			    //if (fabs (static_cast<double>(p1.z ())) < 0.1)
			    //{
			    corners.push_back (SbVec3f (static_cast<double>(p1.x ()), 
							static_cast<double>(p1.y ()),
							static_cast<double>(p1.z ())));		    
			    IgV3d p2  = m.get<IgV3d> ("front_2");
			    corners.push_back (SbVec3f (static_cast<double>(p2.x ()), 
							static_cast<double>(p2.y ()),
							static_cast<double>(p2.z ())));
			    IgV3d p3  = m.get<IgV3d> ("front_3");
			    corners.push_back (SbVec3f (static_cast<double>(p3.x ()), 
							static_cast<double>(p3.y ()),
							static_cast<double>(p3.z ())));
			    IgV3d p4  = m.get<IgV3d> ("front_4");
			    corners.push_back (SbVec3f (static_cast<double>(p4.x ()), 
							static_cast<double>(p4.y ()),
							static_cast<double>(p4.z ())));
			    IgV3d p5  = m.get<IgV3d> ("back_1");
			    corners.push_back (SbVec3f (static_cast<double>(p5.x ()), 
							static_cast<double>(p5.y ()),
							static_cast<double>(p5.z ())));
			    IgV3d p6  = m.get<IgV3d> ("back_2");
			    corners.push_back (SbVec3f (static_cast<double>(p6.x ()), 
							static_cast<double>(p6.y ()),
							static_cast<double>(p6.z ())));
			    IgV3d p7  = m.get<IgV3d> ("back_3");
			    corners.push_back (SbVec3f (static_cast<double>(p7.x ()), 
							static_cast<double>(p7.y ()),
							static_cast<double>(p7.z ())));
			    IgV3d p8  = m.get<IgV3d> ("back_4");
			    corners.push_back (SbVec3f (static_cast<double>(p8.x ()), 
							static_cast<double>(p8.y ()),
							static_cast<double>(p8.z ())));	    
			    lineIndices.push_back (i);
			    lineIndices.push_back (i + 1);
			    lineIndices.push_back (i + 3);
			    lineIndices.push_back (i + 2);
			    lineIndices.push_back (i);
			    lineIndices.push_back (SO_END_LINE_INDEX);
		    
			    lineIndices.push_back (i + 4);
			    lineIndices.push_back (i + 5);
			    lineIndices.push_back (i + 7);
			    lineIndices.push_back (i + 6);
			    lineIndices.push_back (i + 4);
			    lineIndices.push_back (SO_END_LINE_INDEX);
			    i += 8;
			    // i += 4;			
			    //}		    
			}
		    }
		}
		    
		vertices->vertex.setValues (0, corners.size (), &corners [0]);
		vertices->vertex.setNum (corners.size ());	    
		    
		SoIndexedLineSet *lineSet = new SoIndexedLineSet;
		lineSet->coordIndex.setValues (0, lineIndices.size (), &lineIndices [0]);
		lineSet->vertexProperty = vertices;
		sep->addChild (lineSet);
	    }
	}
    }
}