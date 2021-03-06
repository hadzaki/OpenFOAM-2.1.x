    word constProp(initialConditions.lookup("constantProperty"));
    if ((constProp != "pressure") && (constProp != "volume"))
    {
        FatalError << "in initialConditions, unknown constantProperty type "
            << constProp << nl << " Valid types are: pressure volume."
            << abort(FatalError);
    }

    word fractionBasis(initialConditions.lookup("fractionBasis"));
    if ((fractionBasis != "mass") && (fractionBasis != "mole"))
    {
        FatalError << "in initialConditions, unknown fractionBasis type " << nl
            << "Valid types are: mass or mole."
            << fractionBasis << abort(FatalError);
    }

    label nSpecie = Y.size();
    PtrList<gasThermoPhysics> specieData(Y.size());
    forAll(specieData, i)
    {
        specieData.set
        (
            i,
            new gasThermoPhysics
            (
                dynamic_cast<const reactingMixture<gasThermoPhysics>&>
                    (thermo).speciesData()[i]
            )
        );
    }

    scalarList Y0(nSpecie, 0.0);
    scalarList X0(nSpecie, 0.0);

    dictionary fractions(initialConditions.subDict("fractions"));
    if (fractionBasis == "mole")
    {
        forAll(Y, i)
        {
            const word& name = Y[i].name();
            if (fractions.found(name))
            {
                X0[i] = readScalar(fractions.lookup(name));
            }
        }

        scalar mw = 0.0;
        const scalar mTot = sum(X0);
        forAll(Y, i)
        {
            X0[i] /= mTot;
            mw += specieData[i].W()*X0[i];
        }

        forAll(Y, i)
        {
            Y0[i] = X0[i]*specieData[i].W()/mw;
        }
    }
    else  // mass fraction
    {
        forAll(Y, i)
        {
            const word& name = Y[i].name();
            if (fractions.found(name))
            {
                Y0[i] = readScalar(fractions.lookup(name));
            }
        }

        scalar invW = 0.0;
        const scalar mTot = sum(Y0);
        forAll(Y, i)
        {
            Y0[i] /= mTot;
            invW += Y0[i]/specieData[i].W();
        }
        const scalar mw = 1.0/invW;

        forAll(Y, i)
        {
            X0[i] = Y0[i]*mw/specieData[i].W();
        }
    }

    scalar hs0 = 0.0;
    forAll(Y, i)
    {
        Y[i] = Y0[i];
        hs0 += Y0[i]*specieData[i].Hs(T0);
    }

    hs = dimensionedScalar("hs", dimEnergy/dimMass, hs0);

    thermo.correct();

    rho = thermo.rho();
    scalar rho0 = rho[0];
    scalar u0 = hs0 - p0/rho0;
    scalar R0 = p0/(rho0*T0);
    Rspecific[0] = R0;
 
    scalar integratedHeat = 0.0;

    Info << constProp << " will be held constant." << nl
        << " p   = " << p[0] << " [Pa]" << nl
        << " T   = " << thermo.T()[0] << " [K] " << nl
        << " rho = " << rho[0] << " [kg/m3]" << nl
        << endl;


