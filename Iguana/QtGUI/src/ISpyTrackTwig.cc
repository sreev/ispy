#define QT_NO_EMIT

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpyTrackTwig.h"
#include "Iguana/QtGUI/interface/ISpyReadService.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Inventor/interface/IgSoSplineTrack.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include <Inventor/nodes/SoMarkerSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <QString>
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

ISpyTrackTwig::ISpyTrackTwig (IgState *state, IgTwig *parent,
			      const std::string &name /* = "" */)
    : ISpyQueuedTwig (state, parent, name)
{}

void
ISpyTrackTwig::onNewEvent (ISpyEventMessage& message)
{
    ISpyQueuedTwig::onNewEvent (message);

    QSettings settings;    
    QString visSettings ("igtwigs/visibility/");
    visSettings.append ("Tracks_V1");

    if (settings.contains (visSettings) && 
	Qt::CheckState (settings.value (visSettings).value<int> ()) == Qt::Unchecked)
	return;

    if (ISpyReadService *readService = ISpyReadService::get (state ()))
    {	
	IgDataStorage *storage = readService->dataStorage ();
	if (storage->hasCollection ("Tracks_V1") && storage->hasCollection ("Extras_V1"))
	{	    
	    IgCollection &tracks = storage->getCollection ("Tracks_V1");
	    if (tracks.size () > 0 && tracks.hasProperty ("pt") && tracks.hasProperty ("pos") && tracks.hasProperty ("dir"))
	    {    
		IgProperty PT  = tracks.getProperty ("pt");
		IgProperty POS = tracks.getProperty ("pos");
		IgProperty DIR = tracks.getProperty ("dir");

		IgAssociationSet &trackExtras = storage->getAssociationSet ("TrackExtras_V1");
		IgCollection &extras = storage->getCollection ("Extras_V1");

		SoSeparator *sep = new SoSeparator;
		sep->setName (SbName ("Tracks_V1"));
	    
		SoMaterial *mat = new SoMaterial;
		float rgbcomponents [4];
		IgSbColorMap::unpack (0x99ccff, rgbcomponents);
		mat->diffuseColor.setValue (SbColor (rgbcomponents));
		sep->addChild (mat);

		SoDrawStyle *sty = new SoDrawStyle;
		sty->style = SoDrawStyle::LINES;
		sty->lineWidth.setValue (3);
		sep->addChild (sty);

		SoSeparator *vsep = new SoSeparator;
		sep->addChild (vsep);
	    
		SoMaterial *vmat = new SoMaterial;
		IgSbColorMap::unpack (0x003366, rgbcomponents);
		vmat->diffuseColor.setValue (SbColor (rgbcomponents));
		vsep->addChild (mat);

		int nv = 0;
		SoVertexProperty *vertices = new SoVertexProperty;
		IgCollectionIterator cit = tracks.begin ();
		IgCollectionIterator cend = tracks.end ();
		for (; cit != cend ; cit++) 
		{
		    IgSoSplineTrack* trackRep = new IgSoSplineTrack;
		    SoVertexProperty *tvertices = new SoVertexProperty;

		    SoMFVec3f tangents;
		    SoMFVec3f points;
		    int nVtx = 0;

		    IgCollectionItem itrack = *cit;
		    IgV3d p1  = itrack.get<IgV3d> (POS);
		    double x = static_cast<double>(p1.x());
		    double y = static_cast<double>(p1.y());
		    double z = static_cast<double>(p1.z());
		    vertices->vertex.set1Value (nv, SbVec3f (x, y, z));
		    ++nv;
		    
		    QString trackName = QString ("Track %1 GeV (%2, %3, %4)")
					.arg (itrack.get<double>(PT))
					.arg (x)
					.arg (y)
					.arg (z);
		    IgV3d d1  = itrack.get<IgV3d> (DIR);
		    double dx = static_cast<double>(d1.x());
		    double dy = static_cast<double>(d1.y());
		    double dz = static_cast<double>(d1.z());
		
		    for (IgAssociationSet::Iterator a = trackExtras.begin ();
			 a != trackExtras.end ();
			 ++a)
		    {
			if (a->first ().objectId () == itrack.currentRow ())
			{
			    IgCollectionItem m (&extras, a->second ().objectId ());
			    p1 = m.get<IgV3d> ("pos_1");
			    x = static_cast<double>(p1.x());
			    y = static_cast<double>(p1.y());
			    z = static_cast<double>(p1.z());
			    d1 = m.get<IgV3d> ("dir_1");
			    dx = static_cast<double>(d1.x());
			    dy = static_cast<double>(d1.y());
			    dz = static_cast<double>(d1.z());
			    SbVec3f diri (dx, dy, dz);
			    diri.normalize ();
			
			    points.set1Value (nVtx, SbVec3f (x, y, z));
			    tangents.set1Value (nVtx, diri);
			    tvertices->vertex.set1Value (nVtx, SbVec3f (x, y, z));
			    ++nVtx;

			    p1 = m.get<IgV3d> ("pos_2");
			    x = static_cast<double>(p1.x());
			    y = static_cast<double>(p1.y());
			    z = static_cast<double>(p1.z());
			    d1 = m.get<IgV3d> ("dir_2");
			    dx = static_cast<double>(d1.x());
			    dy = static_cast<double>(d1.y());
			    dz = static_cast<double>(d1.z());
			    SbVec3f diro (dx, dy, dz);
			    diro.normalize ();
			
			    points.set1Value (nVtx, SbVec3f (x, y, z));
			    tangents.set1Value (nVtx, diro);
			    tvertices->vertex.set1Value (nVtx, SbVec3f (x, y, z));
			    ++nVtx;
			
			    trackRep->points = points;
			    trackRep->tangents = tangents;
			    sep->addChild (trackRep);
			}
		    }
		    tvertices->vertex.setNum (nVtx);
		    SoMFInt32 tmarkerIndex;
		    tmarkerIndex.setValue (SoMarkerSet::CIRCLE_LINE_7_7);
		
		    SoMarkerSet *tpoints = new SoMarkerSet;
		    tpoints->markerIndex = tmarkerIndex;
		    tpoints->vertexProperty.setValue (tvertices);
		    tpoints->numPoints.setValue (nVtx);
		
		    sep->addChild (tpoints);
		}
		
		vertices->vertex.setNum (nv);
		    
		SoMFInt32 markerIndex;
		markerIndex.setValue (SoMarkerSet::CROSS_9_9);
		
		SoMarkerSet *mpoints = new SoMarkerSet;
		mpoints->markerIndex = markerIndex;
		mpoints->vertexProperty.setValue (vertices);
		mpoints->numPoints.setValue (nv);
		
		vsep->addChild (mpoints);
		
		dynamic_cast<SoSeparator *>(ISpyQueuedTwig::rep ())->addChild (sep);		
	    }
	}
    }
}