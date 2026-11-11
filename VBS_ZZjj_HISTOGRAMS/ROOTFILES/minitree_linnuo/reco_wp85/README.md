# VBS ZZ reco_wp85 mini-trees



## Reconstruction selection

The mini-trees were produced with `runZZ --filter general`:

```cpp
nbaselineLep >= 4
&& num_sfos_pair >= 2
&& nj_good >= 2
```

No `pass_lep_eta` requirement was applied. The selected-jet collection combines
`jet_*` and `fjet_*` and requires:

```cpp
std::abs(eta) < 4.5 && pT > 30 GeV
```

The tagging pair is the selected dijet with the highest scalar-pT sum.
Opposite-side, trigger, signal-lepton, `mjj`, and `delta_y_jj` requirements
are applied downstream rather than by the reconstruction filter.

The upstream object definitions below are taken from
`MasterShef/share/confs/vbszz_Run3_lite.conf`. Signal object containers are
formed after overlap removal.

## Object selection

Here `z0` denotes MasterShef's `z0sinTheta` quantity. The baseline
`z0=0.5` values are the documented SUSYTools fallbacks when no explicit
`EleBaseline.z0` or `MuonBaseline.z0` key is set.

| Object | Baseline | Signal |
| --- | --- | --- |
| Electron | $p_{\mathrm{T}}>7$ GeV; $\lvert\eta\rvert<2.47$; `VeryLooseLLH`;no crack veto; no $d_0$-significance cut; $\lvert z_0\sin\theta\rvert<0.5$ mm (SUSYTools default). | $E_{\mathrm{T}}>7$ GeV; $\lvert\eta\rvert<2.47$; `LooseAndBLayerLLH`; `Loose_VarRad` isolation, including above 200 GeV; no crack veto; $\lvert d_0\rvert/\sigma(d_0)<5$; $\lvert z_0\sin\theta\rvert<0.5$ mm;  |
| Muon | $p_{\mathrm{T}}>5$ GeV; $\lvert\eta\rvert<2.7$; `Loose`; no isolation; no $d_0$-significance cut; $\lvert z_0\sin\theta\rvert<0.5$ mm (SUSYTools default). | $p_{\mathrm{T}}>5$ GeV; $\lvert\eta\rvert<2.7$; `Loose`; `PflowLoose_VarRad` isolation, including above 200 GeV; $\lvert d_0\rvert/\sigma(d_0)<3$; $\lvert z_0\sin\theta\rvert<0.5$ mm. |



## Indexing

- `lepton_*` contains reconstructed leptons in descending global \(p_{\mathrm{T}}\) order.  Vector `pt` and `e` are in MeV.
- `sfos_type` stores one-based indices into `lepton_*`; for example, `"13"` refers to vector entries 0 and 2.
- The first two disjoint SFOS pairs define the selected ZZ quadruplet. `sfos_type_1` is the pair closest to \(m_Z=91.1876\) GeV.
- `pT_1lep...pT_4lep` are the selected ZZ leptons reordered by \(p_{\mathrm{T}}\). `m_ll_12` and `m_ll_34` follow Z-pair order, rather than this \(p_{\mathrm{T}}\) order.
- `tagjet1` and `tagjet2` are the selected tagging-dijet members ordered by \(p_{\mathrm{T}}\); they need not be the global leading two selected jets.

## Downstream SR/CR selection

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
python3 scripts/plot_vbszz_stacks.py \\
  -i /lustre/umt3/user/linnuo/VBSZZ/skim/reco_wp85/mc \\
     /lustre/umt3/user/linnuo/VBSZZ/skim/reco_wp85/fake \\
  --data-input /lustre/umt3/user/linnuo/VBSZZ/skim/reco_wp85/data \\
  --weight-scale 163855 \\
  --outdir <output_dir>
```

MC uses `EventWeight * 163855`; data uses unit weight.
