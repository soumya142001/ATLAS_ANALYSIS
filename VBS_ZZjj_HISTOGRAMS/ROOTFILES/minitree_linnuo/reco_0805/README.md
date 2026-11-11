# VBS ZZ merged minitrees

These files contain reconstructed VBS `ZZ -> 4l + 2j` minitrees.

- Tree: `Nominal`
- MC files: `mc/`
- Data files: `data/`
- MC plotting weight: `EventWeight * 163855`
- Data plotting weight: `1`
- `minitree_files.list` contains paths relative to this directory.

## Reconstruction filter already applied

These minitrees were produced with the `general` reconstruction filter:

```
nbaselineLep >= 4 && num_sfos_pair >= 2 && nj_good >= 2 && has_dijet
```

The selected jet collection combines the input `jet_*` and `fjet_*` collections and requires:

```
abs(eta) < 4.5
pT > 30 GeV  for abs(eta) <= 2.4
pT > 40 GeV  for 2.4 < abs(eta) < 4.5
```

`has_dijet` means that an opposite-side jet pair is available. Among all opposite-side pairs, the pair with the highest scalar pT sum is selected. 

## Lepton and SFOS indices

`pT_1lep...pT_4lep` and the corresponding scalar `eta`, `phi`, `charge`, and `ifftype` branches describe the four leptons used by Z1 and Z2, reordered by descending pT inside the selected quadruplet. Their pT values are in GeV. 

`zz_leptons_are_top4` is true when these four leptons are also the global first four entries of `lepton_*`.

The full `lepton_pt`, `lepton_eta`, `lepton_phi`, `lepton_e`, `lepton_charge`, `lepton_flavor`, `lepton_z0sinTheta`, and `lepton_d0sig` vectors use the same global descending-pT order. Vector `lepton_pt` and `lepton_e` are stored in MeV. 

`sfos_type` uses 1-based indices into these full `lepton_*` vectors. For example, `"13"` means `lepton_*[0]` and `lepton_*[2]`. `sfos_type_1` is the selected SFOS pair closest to `mZ = 91.1876 GeV`; `sfos_type_2` is the next selected pair without reusing either Z1 lepton. `num_sfos_pair` is the number of selected disjoint pairs, not the number of all combinatorial SFOS pairs.


The indices in pair-derived names use a different convention:

```text
1 = first lepton in Z1
2 = second lepton in Z1
3 = first lepton in Z2
4 = second lepton in Z2
```

Therefore `m_ll_12` and `dR_ll_12` describe Z1, while `m_ll_34` and `dR_ll_34` describe Z2. Cross-pair quantities such as `m_ll_14`, `m_ll_23`, and `dR_ll_13/14/23/24` use this Z1/Z2 pair-order convention. These numbers do not refer to the pT ranks in `pT_1lep...pT_4lep`.

`dR_sfos_12`, `dEta_sfos_12` and `dPhi_sfos_12` are dR, dEta, and dPhi between 2reconstructed Z candidates.

## Jet indices

 `tagjet1` and `tagjet2` form the selected highest-scalar-pT-sum opposite-side dijet; `tagjet1` is the higher-pT member of that pair. The tagging jets are therefore not guaranteed to be the global leading two selected jets.

 `pT_1jet`, `pT_2jet`, and the corresponding jet branches are the globally pT-ordered selected jets.

`mjj`, `delta_y_jj`, and `yj1_yj2` are calculated from `tagjet1 + tagjet2`. Use `std::abs(delta_y_jj)` when the absolute rapidity separation is needed.

## Simple post-reconstruction cuts

The current loose validation selection can be applied directly with RDataFrame strings:

```cpp
std::string common =
    "passTriggers"
    " && pT_1lep>20 && pT_2lep>20 && pT_3lep>10 && pT_4lep>7"
    " && m_ll_12>66 && m_ll_12<116"
    " && m_ll_34>66 && m_ll_34<116";

std::string sr =
    common + " && std::abs(delta_y_jj)>2 && mjj>300";

std::string qcd_cr =
    common + " && (mjj<300 || std::abs(delta_y_jj)<2)";

ROOT::RDataFrame df("Nominal", inputFiles);
auto df_sr = df.Filter(sr);
auto df_qcd_cr = df.Filter(qcd_cr);
```
