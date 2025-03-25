#!/bin/bash
# AmberTools test script

# Create a directory for the test
mkdir -p ambertools_test
cd ambertools_test

# Create a simple water molecule in LEaP
cat > water.leap << EOF
# Initialize LEaP
source leaprc.water.tip3p

# Create a water molecule
m = loadPdb "water.pdb"

# Save parameter and topology files
saveAmberParm m water.prmtop water.inpcrd

# Quit LEaP
quit
EOF

# Create a simple water PDB file
cat > water.pdb << EOF
ATOM      1  O   WAT     1       0.000   0.000   0.000  1.00  0.00
ATOM      2  H1  WAT     1       0.957   0.000   0.000  1.00  0.00
ATOM      3  H2  WAT     1      -0.240   0.927   0.000  1.00  0.00
TER
END
EOF

# Run LEaP to generate topology and coordinates
tleap -f water.leap > water_leap.log 2>&1

# Check if files were created successfully
if [ -f water.prmtop ] && [ -f water.inpcrd ]; then
    echo "SUCCESS: LEaP successfully created water.prmtop and water.inpcrd"
else
    echo "ERROR: LEaP failed to create output files"
    cat water_leap.log
    exit 1
fi

# Run a quick minimization with sander
cat > water_min.in << EOF
Water molecule minimization
&cntrl
  imin = 1,
  maxcyc = 100,
  ncyc = 50,
  ntb = 0,
  cut = 999.0,
/
EOF

# Run minimization
sander -O -i water_min.in -o water_min.out -p water.prmtop -c water.inpcrd -r water_min.rst

# Check if minimization was successful
if [ -f water_min.rst ]; then
    echo "SUCCESS: Sander minimization completed"
    grep "FINAL RESULTS" water_min.out
else
    echo "ERROR: Sander minimization failed"
    cat water_min.out
    exit 1
fi

# Run a simple antechamber test
# Create a simple ethane PDB file instead of SMILES
cat > ethane.pdb << EOF
ATOM      1  C1  ETH     1       0.000   0.000   0.000  1.00  0.00
ATOM      2  C2  ETH     1       1.540   0.000   0.000  1.00  0.00
ATOM      3  H11 ETH     1      -0.399  -0.350   0.933  1.00  0.00
ATOM      4  H12 ETH     1      -0.399  -0.667  -0.755  1.00  0.00
ATOM      5  H13 ETH     1      -0.399   1.017  -0.177  1.00  0.00
ATOM      6  H21 ETH     1       1.939   0.350  -0.933  1.00  0.00
ATOM      7  H22 ETH     1       1.939   0.667   0.755  1.00  0.00
ATOM      8  H23 ETH     1       1.939  -1.017   0.177  1.00  0.00
TER
END
EOF

antechamber -i ethane.pdb -fi pdb -o ethane.mol2 -fo mol2 -c bcc -nc 0

# Check if antechamber was successful
if [ -f ethane.mol2 ]; then
    echo "SUCCESS: Antechamber successfully created ethane.mol2"
    head -n 10 ethane.mol2
else
    echo "ERROR: Antechamber failed"
    exit 1
fi

# Clean up
cd ..

echo "All tests completed successfully!"
