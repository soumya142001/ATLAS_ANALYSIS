# VBS ZZ Merged Minitrees

This directory contains the no-selected-lepton-eta-cut `runZZ` production. MC and fake-background samples are merged by DSID across the three Run 3 campaigns. Data remains separated into 2022, 2023, and 2024 files. Polarized ZZ samples are kept in `polar/` by DSID and campaign.

## Contents

```text
merged/
  mc/       prompt MC, one ROOT file per DSID
  fake/     Z+jets, ttbar, and other fake-background MC, one file per DSID
  data/     one ROOT file per data-taking year
  polar/    polarized ZZ TT/TL/LL minitrees, one ROOT file per campaign
```

The tree name is `Nominal`. `minitree_files.list` contains commented relative paths; `minitree_files.paths_only.list` can be read directly by scripts. `merge_validation.tsv` records input-file counts, entries, and `sum(EventWeight)` for every merged output.

The files were merged with `ROOT.TFileMerger`, following `scripts/merger.py`. All input files are retained. For every output, the summed entries of each `TTree` and `TH1` key and the `Nominal` `EventWeight` sum were checked against the inputs.

## Reconstruction Selection

The minitrees were produced with `runZZ --filter general`:

```cpp
nbaselineLep >= 4
&& num_sfos_pair >= 2
&& nj_good >= 2
```

No `pass_lep_eta` requirement was applied. The selected-jet collection combines `jet_*` and `fjet_*` and requires:

```cpp
std::abs(eta) < 4.5 && pT > 30 GeV
```

The tagging pair is the selected dijet with the highest scalar pT sum. Opposite-side, trigger, signal-lepton, `mjj`, and `delta_y_jj` requirements are applied downstream rather than by the reconstruction filter.

## Indexing

- `lepton_*` contains all reconstructed leptons in global descending pT order. Vector `pt` and `e` are in MeV.
- `sfos_type` stores one-based indices into `lepton_*`; `"13"` means vector entries 0 and 2.
- The first two disjoint SFOS pairs define the selected ZZ quadruplet. `sfos_type_1` is closest to `mZ = 91.1876 GeV`.
- `pT_1lep...pT_4lep` are the selected ZZ leptons reordered by pT; scalar pT is in GeV.
- `m_ll_12` and `m_ll_34` are the selected Z1 and Z2 masses. Their labels follow the pair order, not the `pT_1lep...4lep` order.
- `tagjet1` and `tagjet2` are the chosen dijet members ordered by pT. They need not equal the global leading `pT_1jet` and `pT_2jet`.

## Downstream Selection

The current stack workflow uses:

```cpp
std::string common =
    "has_dijet && yj1_yj2 < 0 && passTriggers"
    " && pT_1lep > 20 && pT_2lep > 20"
    " && m_ll_12 > 5 && m_ll_34 > 5"
    " && min_dR_4l > 0.05 && nsignalLep == 4";

std::string sr =
    common + " && std::abs(delta_y_jj) > 2 && mjj > 300";

std::string qcd_cr =
    common + " && (mjj < 300 || std::abs(delta_y_jj) < 2)";
```

Run the stack plotter from the AnaSkim repository root:

```bash
python3 scripts/plot_vbszz_stacks.py \
  -i <merged_dir>/mc <merged_dir>/fake \
  --data-input <merged_dir>/data \
  --weight-scale 163855 \
  --outdir <output_dir>
```

MC uses `EventWeight * 163855`; Data uses unit weight. Do not apply an additional campaign factor after merging.
