

```bash
➜ apptainer exec ambertools24.aimg ./ambertools-test.sh 
SUCCESS: LEaP successfully created water.prmtop and water.inpcrd
SUCCESS: Sander minimization completed
                    FINAL RESULTS
Info: acdoctor mode is on: check and diagnose problems in the input file.
Info: The atom type is set to gaff; the options available to the -at flag are
      gaff, gaff2, amber, bcc, abcg2, and sybyl.

-- Check Format for pdb File --
   Status: pass
Info: Total number of electrons: 18; net charge: 0

Running: /opt/conda/envs/ambertools/bin/sqm -O -i sqm.in -o sqm.out

SUCCESS: Antechamber successfully created ethane.mol2
@<TRIPOS>MOLECULE
ETH
    8     7     1     0     0
SMALL
bcc


@<TRIPOS>ATOM
      1 C1           0.0000     0.0000     0.0000 c3         1 ETH      -0.094100
      2 C2           1.5400     0.0000     0.0000 c3         1 ETH      -0.094100
All tests completed successfully!
```
