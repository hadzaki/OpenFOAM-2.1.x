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
    Foam::fvMeshAdder

Description
    Adds two fvMeshes without using any polyMesh morphing.
    Uses fvMeshAdder.

SourceFiles
    fvMeshAdder.C
    fvMeshAdderTemplates.C

\*---------------------------------------------------------------------------*/

#ifndef fvMeshAdder_H
#define fvMeshAdder_H

#include "polyMeshAdder.H"
#include "fvPatchFieldsFwd.H"
#include "fvsPatchFieldsFwd.H"
#include "fvPatchFieldMapper.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes
class IOobject;
class faceCoupleInfo;
class IOobjectList;
class fvMesh;
class volMesh;
class surfaceMesh;
class mapAddedPolyMesh;

/*---------------------------------------------------------------------------*\
                           Class fvMeshAdder Declaration
\*---------------------------------------------------------------------------*/

class fvMeshAdder
:
    public polyMeshAdder
{

private:


    // Private class

    class directFvPatchFieldMapper
    :
        public fvPatchFieldMapper
    {
        // Private data

            const labelList& directAddressing_;

    public:

        // Constructors

            //- Construct from components
            directFvPatchFieldMapper(const labelList& directAddressing)
            :
                fvPatchFieldMapper(),
                directAddressing_(directAddressing)
            {}


        //- Destructor
        virtual ~directFvPatchFieldMapper()
        {}


        // Member Functions

            label size() const
            {
                return directAddressing_.size();
            }

            bool direct() const
            {
                return true;
            }

            const labelUList& directAddressing() const
            {
                return directAddressing_;
            }
    };


    // Private Member Functions

        //- Calculate map from new patch faces to old patch faces. -1 where
        //  could not map.
        static labelList calcPatchMap
        (
            const label oldStart,
            const label oldSize,
            const labelList& oldToNew,
            const polyPatch& newPatch,
            const label unmappedIndex
        );

        //- Map from old to new according to map. Handles map = -1.
        template<class Type>
        static void map
        (
            const Field<Type>&,
            const labelList& map,
            Field<Type>&
        );

        //- Update single volField.
        template<class Type>
        static void MapVolField
        (
            const mapAddedPolyMesh& meshMap,

            GeometricField<Type, fvPatchField, volMesh>& fld,
            const GeometricField<Type, fvPatchField, volMesh>& fldToAdd
        );

        //- Update single surfaceField.
        template<class Type>
        static void MapSurfaceField
        (
            const mapAddedPolyMesh& meshMap,

            GeometricField<Type, fvsPatchField, surfaceMesh>& fld,
            const GeometricField<Type, fvsPatchField, surfaceMesh>& fldToAdd
        );

public:

    // Member Functions

        //- Inplace add mesh to fvMesh. Maps all stored fields. Returns map.
        static autoPtr<mapAddedPolyMesh> add
        (
            fvMesh& mesh0,
            const fvMesh& mesh1,
            const faceCoupleInfo& coupleInfo,
            const bool validBoundary = true
        );

        //- Map all volFields of Type
        template<class Type>
        static void MapVolFields
        (
            const mapAddedPolyMesh&,
            const fvMesh& mesh,
            const fvMesh& meshToAdd
        );

        //- Map all surfaceFields of Type
        template<class Type>
        static void MapSurfaceFields
        (
            const mapAddedPolyMesh&,
            const fvMesh& mesh,
            const fvMesh& meshToAdd
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "fvMeshAdderTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
