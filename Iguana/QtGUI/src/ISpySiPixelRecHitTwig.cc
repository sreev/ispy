#define QT_NO_EMIT

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpySiPixelRecHitTwig.h"
#include "Iguana/QtGUI/interface/ISpyReadService.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMarkerSet.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <QSettings>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

ISpySiPixelRecHitTwig::ISpySiPixelRecHitTwig (IgState *state, IgTwig *parent, 
					      const std::string &name)
    : ISpyQueuedTwig (state, parent, name)
{}

void
ISpySiPixelRecHitTwig::onNewEvent (ISpyEventMessage& message) 
{    
    ISpyQueuedTwig::onNewEvent (message);

    QSettings settings;    
    QString visSettings ("igtwigs/visibility/");
    visSettings.append ("SiPixelRecHits_V1");

    if (settings.contains (visSettings) && 
	Qt::CheckState (settings.value (visSettings).value<int> ()) == Qt::Unchecked)
	return;

    if (ISpyReadService *readService = ISpyReadService::get (state ()))
    {	
	IgDataStorage *storage = readService->dataStorage ();
	if (storage->hasCollection ("SiPixelRecHits_V1"))
	{	    
	    IgCollection &rechits = storage->getCollection ("SiPixelRecHits_V1");
	    if (rechits.size () > 0 && rechits.hasProperty ("pos"))
	    {
		IgProperty POS = rechits.getProperty ("pos");

		int n = 0;
		SoVertexProperty *vertices = new SoVertexProperty;

		IgCollectionIterator cit = rechits.begin ();
		IgCollectionIterator cend = rechits.end ();
		for (; cit != cend ; cit++, n++) 
		{
		    IgCollectionItem m = *cit;

		    IgV3d p1 = m.get<IgV3d> (POS);
		
		    double x = p1.x ();
		    double y = p1.y ();
		    double z = p1.z ();
		    vertices->vertex.set1Value (n, SbVec3f (x, y, z));
		}
		vertices->vertex.setNum (n);

		SoSeparator *sep = dynamic_cast<SoSeparator *>(ISpyQueuedTwig::rep ());
		sep->setName (SbName ("SiPixelRecHits_V1"));

		SoMaterial *mat = new SoMaterial;
		//float rgbcomponents [4];
		//IgSbColorMap::unpack (0xB0E57C00, rgbcomponents);
		//mat->diffuseColor.setValue (SbColor (rgbcomponents));
		 mat->diffuseColor.setValue (1.0, 0.0, 0.0);
		sep->addChild (mat);

		SoMFInt32 tmarkerIndex;
		tmarkerIndex.setValue (SoMarkerSet::PLUS_7_7);
		
		SoMarkerSet *sopoints = new SoMarkerSet;
		sopoints->markerIndex = tmarkerIndex;
		sopoints->vertexProperty.setValue (vertices);
		sopoints->numPoints.setValue (n);
	  
		sep->addChild (sopoints);
	    }
	}
    }
}