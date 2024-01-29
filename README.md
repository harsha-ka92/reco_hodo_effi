# Calculating hodoscope efficiencies using reconstructed tracks

This module was originally developed by [Kun Liu](https://github.com/liuk) to analyze the efficiencies of proportional tubes.

```bash
git clone https://github.com/dinupa1/reco_hodo_effi.git
cd reco_hodo_effi
source setup.sh
rebuild-ana
```

## Make simple tree

In this section, we read the reconstructed output file and create a simple tree structure. For example, let's assume you are analyzing run `4544`. Use the following command to generate a list of directories containing the file `data.root`:

```bash
make-reco-list /pnfs/e1039/persistent/cosmic_recodata/run_004544
```

This command will create a text file (`list_reco.txt`), which we will use as input for the `run.C` file. Execute the following commands to obtain the simple tree:

```bash
root -l -b -q run.C
```

This will generate the file `ana.root`, which we will utilize in the next section.

## Plane efficiencies

In this section, we analyze the file `ana.root` to calculate the plane efficiencies. Plots will be saved in the `imgs/planEffi` directory, and an output file will be created for use in the next section.

```
root -l -b -q planeEffi.cc
```

## Paddle efficiency

In this section, we analyze the file `plane_effi.root` to calculate the paddle efficiencies. Plots will be saved in the `imgs/planEffi` directory.

```
root -l -b -q paddleEffi.cc

## Noise

### Development

Kindly submit a pull request.
