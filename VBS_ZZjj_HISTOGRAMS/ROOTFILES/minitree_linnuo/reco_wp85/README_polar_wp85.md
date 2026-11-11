# Run-3 polarized ZZ minitrees — continuous b-tag WP85
 

## Files and short names

The copy script in this directory writes the files to `EWK_polar/` or
`QCD_polar/` with this name pattern:

```text
{QCD|EWK}_{TT|TL|LL}_{DSID}_{r-tag}.root
```

The `r-tag` is retained in full because it identifies the Run-3 production
campaign.  The three tags are `r15530-r15514`, `r15540-r15516`, and
`r16083-r15970`.  The common information omitted from the short name is
`mc23_13p6TeV`, `DAOD_PHYSLITE`, `e8588_e8586`, and `p7266`; it is shared by
all files and is retained by the original source filename.

| Process | Polarization | DSID | Destination short-name pattern |
|---|---|---:|---|
| QCD ZZjj | TT | 546884 | `QCD_TT_546884_{r-tag}.root` |
| QCD ZZjj | TL | 546885 | `QCD_TL_546885_{r-tag}.root` |
| QCD ZZjj | LL | 546886 | `QCD_LL_546886_{r-tag}.root` |
| EWK ZZjj | TT | 546887 | `EWK_TT_546887_{r-tag}.root` |
| EWK ZZjj | TL | 546888 | `EWK_TL_546888_{r-tag}.root` |
| EWK ZZjj | LL | 546889 | `EWK_LL_546889_{r-tag}.root` |

There are nine ROOT files per process: three polarizations times three
Run-3 tags.

## Minitree baseline selection

The `general` filter applied during minitree production is:

```text
nbaselineLep >= 4
&& num_sfos_pair >= 2
&& nj_good >= 2
```

`nj_good` combines central `jet_*` and forward `fjet_*`, with
`pT > 30 GeV` and `|eta| < 4.5`.  `num_bjets` counts central b-tagged jets
with `pT > 20 GeV` and `|eta| < 2.5`; it uses continuous b-tag WP85.

## Plot-level cuts

Common cut:

```text
has_dijet && yj1_yj2 < 0.0 && passTriggers
&& pT_1lep > 20.0 && pT_2lep > 20.0
&& m_ll_12 > 5.0 && m_ll_34 > 5.0
&& min_dR_4l > 0.05 && nsignalLep == 4
```

Signal region (SR):

```text
COMMON_CUT && abs(delta_y_jj) > 2.0 && mjj > 300.0
```

QCD control region (CR):

```text
COMMON_CUT && (mjj < 300.0 || abs(delta_y_jj) < 2.0)
```

No `num_bjets == 0` veto is included in these plotting cuts.

## Weighted yields

Yields use `EventWeight * 163855`; they include the common and region cuts
above, and do not apply a b-jet veto.

| Sample | Polarization | SR yield | QCD CR yield |
|---|---|---:|---:|
| QCD | TT | 26.554 | 42.552 |
| QCD | TL | 12.399 | 19.550 |
| QCD | LL | 2.463 | 3.867 |
| QCD | Total | 41.416 | 65.969 |
| EWK | TT | 13.506 | 2.094 |
| EWK | TL | 6.237 | 0.946 |
| EWK | LL | 1.826 | 0.271 |
| EWK | Total | 21.569 | 3.311 |
