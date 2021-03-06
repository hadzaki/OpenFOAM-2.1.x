/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::treeDataCell

Description
    Encapsulation of data needed to search in/for cells. Used to find the
    cell containing a point (e.g. cell-cell mapping).

SourceFiles
    treeDataCell.C

\*---------------------------------------------------------------------------*/

#ifndef treeDataCell_H
#define treeDataCell_H

#include "polyMesh.H"
#include "treeBoundBoxList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes
template<class Type> class indexedOctree;

/*---------------------------------------------------------------------------*\
                        Class treeDataCell Declaration
\*---------------------------------------------------------------------------*/

class treeDataCell
{
    // Private data

        const polyMesh& mesh_;

        //- Subset of cells to work on
        const labelList cellLabels_;

        //- Whether to precalculate and store cell bounding box
        const bool cacheBb_;

        //- How to decide if point is inside cell
        const polyMesh::cellRepresentation decompMode_;

        //- cell bounding boxes (valid only if cacheBb_)
        treeBoundBoxList bbs_;


    // Private Member Functions

        //- Calculate cell bounding box
        treeBoundBox calcCellBb(const label cellI) const;

        //- Initialise all member data
        void update();

public:

    // Declare name of the class and its debug switch
    ClassName("treeDataCell");


    // Constructors

        //- Construct from mesh and subset of cells.
        treeDataCell
        (
            const bool cacheBb,
            const polyMesh&,
            const labelUList&,
            const polyMesh::cellRepresentation decompMode
        );

        //- Construct from mesh and subset of cells, transferring contents
        treeDataCell
        (
            const bool cacheBb,
            const polyMesh&,
            const Xfer<labelList>&,
            const polyMesh::cellRepresentation decompMode
        );

        //- Construct from mesh. Uses all cells in mesh.
        treeDataCell
        (
            const bool cacheBb,
            const polyMesh&,
            const polyMesh::cellRepresentation decompMode
        );


    // Member Functions

        // Access

            inline const labelList& cellLabels() const
            {
                return cellLabels_;
            }

            inline const polyMesh& mesh() const
            {
                return mesh_;
            }

            inline polyMesh::cellRepresentation decompMode() const
            {
                return decompMode_;
            }

            inline label size() const
            {
                return cellLabels_.size();
            }

            //- Get representative point cloud for all shapes inside
            //  (one point per shape)
            pointField shapePoints() const;


        // Search

            //- Get type (inside,outside,mixed,unknown) of point w.r.t. surface.
            //  Only makes sense for closed surfaces.
            label getVolumeType
            (
                const indexedOctree<treeDataCell>&,
                const point&
            ) const
            {
                notImplemented
                (
                    "treeDataCell::getVolumeType"
                    "(const indexedOctree<treeDataCell>&, const point&)"
                );
                return -1;
            }

            //- Does (bb of) shape at index overlap bb
            bool overlaps
            (
                const label index,
                const treeBoundBox& sampleBb
            ) const;

            //- Does shape at index contain sample
            bool contains
            (
                const label index,
                const point& sample
            ) const;

            //- Calculates nearest (to sample) point in shape.
            //  Returns actual point and distance (squared)
            void findNearest
            (
                const labelUList& indices,
                const point& sample,

                scalar& nearestDistSqr,
                label& nearestIndex,
                point& nearestPoint
            ) const;

            //- Calculates nearest (to line) point in shape.
            //  Returns point and distance (squared)
            void findNearest
            (
                const labelUList& indices,
                const linePointRef& ln,

                treeBoundBox& tightest,
                label& minIndex,
                point& linePoint,
                point& nearestPoint
            ) const
            {
                notImplemented
                (
                    "treeDataCell::findNearest"
                    "(const labelUList&, const linePointRef&, ..)"
                );
            }

            //- Calculate intersection of shape with ray. Sets result
            //  accordingly
            bool intersects
            (
                const label index,
                const point& start,
                const point& end,
                point& result
            ) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


#endif

// ************************************************************************* //
