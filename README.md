panstripe
================

<!-- badges: start -->
[![R-CMD-check](https://github.com/gtonkinhill/panstripe/workflows/R-CMD-check/badge.svg)](https://github.com/gtonkinhill/panstripe/actions)
<!-- [![DOI](https://zenodo.org/badge/137083307.svg)](https://zenodo.org/badge/latestdoi/137083307) -->
<!-- badges: end -->


<p align="center">
<img src="https://github.com/gtonkinhill/panaroo/blob/master/panaroo.png" alt="alt text" width="500">
</p>

Panstripe improves the post processing of bacterial pangenome analyses.
In particular it aims to replace the dubious but popular pangenome
accumulation curve. The package is currently under development so
frequent changes are to be expected.

-   [Installation](#installation)
-   [Quick Start](#quick-start)
-   [Citation](#citation)
-   [Output](#output)
-   [Comparing Pangenomes](#comparing-pangenomes)
-   [Open vs Closed](#open-vs-closed)
-   [Rate vs Size](#rate-vs-size)
-   [Plots](#plots)
    -   [Pangenome fit](#pangenome-fit)
    -   [Tree with presence/absence](#tree-with-presenceabsence)
    -   [Inferred ancestral states](#inferred-ancestral-states)
    -   [tSNE](#tsne)
    -   [Accumulation curves](#accumulation-curves)

<!-- README.md is generated from README.Rmd. Please edit that file -->

## Installation

`panstripe` is currently available on GitHub. It can be installed with
`devtools`

``` r
install.packages("remotes")
remotes::install_github("gtonkinhill/panstripe")
```

If you would like to also build the vignette with your installation run:

``` r
remotes::install_github("gtonkinhill/panstripe", build_vignettes = TRUE)
```

## Quick Start

Panstripe takes as input a phylogeny and gene presence/absence matrix in
Rtab format as is produced by
[Panaroo](https://gtonkinhill.github.io/panaroo/#/),
[Roary](https://github.com/sanger-pathogens/Roary),
[PIRATE](https://github.com/SionBayliss/PIRATE) and other prokaryotic
pangenome tools.

``` r
library(panstripe)
library(ape)
set.seed(1234)

### NOTE: here we load example files from the panstripe package. You should replace
### these variables with the relevant paths to the files you are using.
phylo.file.name <- system.file("extdata", "tree.newick", package = "panstripe")
rtab.file.name <- system.file("extdata", "gene_presence_absence.Rtab", package = "panstripe")
### 

# Load files
pa <- read_rtab(rtab.file.name)
tree <- read.tree(phylo.file.name)

# Run panstripe
fit <- panstripe(pa, tree)
#> Running bootstrap...
#> 1% completed2% completed3% completed4% completed5% completed6% completed7% completed8% completed9% completed10% completed11% completed12% completed13% completed14% completed15% completed16% completed17% completed18% completed19% completed20% completed21% completed22% completed23% completed24% completed25% completed26% completed27% completed28% completed29% completed30% completed31% completed32% completed33% completed34% completed35% completed36% completed37% completed38% completed39% completed40% completed41% completed42% completed43% completed44% completed45% completed46% completed47% completed48% completed49% completed50% completed51% completed52% completed53% completed54% completed55% completed56% completed57% completed58% completed59% completed60% completed61% completed62% completed63% completed64% completed65% completed66% completed67% completed68% completed69% completed70% completed71% completed72% completed73% completed74% completed75% completed76% completed77% completed78% completed79% completed80% completed81% completed82% completed83% completed84% completed85% completed86% completed87% completed88% completed89% completed90% completed91% completed92% completed93% completed94% completed95% completed96% completed97% completed98% completed99% completed100% completedDone
```

## Citation

To cite panstripe please use:

## Output

The `panstripe` function generates a list with the following attributes

#### summary

A table indicating a subset of the inferred parameters of the GLM. The
p-values and bootstrap confidence intervals can be used to determine
whether each term in the model is significantly associated with gene
gain and loss.

-   **core** indicates whether the branch lengths in the phylogeny are
    associated with gene gain and loss.

-   **tip** indicates associations with genes observed to occur on the
    tips of the phylogeny. These are usually driven by a combination of
    annotation errors and depending upon the temporal sampling density
    also highly mobile elements that are not observed in multiple
    genomes.

-   **height** indicates whether the rate of gene gain and loss changes
    significantly with the height of a branch. Typically, our ability to
    detect gene exchange events reduces for older ancestral branches.

#### model

The output of fitting the GLM model using `glmmTMB`. This object can be
used to predict how many gene gains and losses we would expect to
observe given the parameters of a particular branch.

#### data

A table with the data used to fit the model. The `acc` column indicates
the inferred number of gene gain/loss events for a branch; the `core`
column is the branch length taken from the phylogeny; the `istip` column
indicates whether the branch occurs at the tip of the phylogeny and the
`height` column indicates the height at which the branch started.

#### bootstrap replicates

A table with the inferred parameters for each of the bootstrap
replicates that were run.

#### tree/pa

The original data provided to the `panstripe` function.

## Comparing Pangenomes

As `panstripe` uses a GLM framework it is straightforward to compare the
slope terms between datasets. The `compare_pangenomes` function
considers the interaction term between the data sets and the `core`,
`tip` and `height` terms.

**IMPORTANT:** It is important that the phylogenies for the pangenomes
being compared are on the same scale. This can be achieved most easily
by using time scaled phylogenies. Alternatively, it is possible to build
a single phylogeny and partition it into each data set.

Here, we simulate two pangenomes with different gene gain and loss
rates, fit the model using `panstripe` and run the `compare_pangenomes`
function.

A significant p-value for the `tip` term indicates that the two
pangenomes differ in the rates of gene presence and absence assigned to
the tips of the phylogeny. This is usually driven by either differences
in the annotation error rates between data sets or differences in the
gain and loss of highly mobile elements that do not persist long enough
to be observed in multiple genomes.

A significant p-value for the `core` term indicates that two data sets
have different rates of gene gain and loss. This can then be
investigated in more detail by considering the estimates of the
underlying parameters of the model using the `plot_dist_params`
function.

``` r
# Simulate a fast gene gain/loss rate with error
sim_fast <- simulate_pan(rate = 0.001, ngenomes = 100)
sim_slow <- simulate_pan(rate = 1e-04, ngenomes = 100)

# Run panstripe
fit_fast <- panstripe(sim_fast$pa, sim_fast$tree)
#> Running bootstrap...
#> 1% completed2% completed3% completed4% completed5% completed6% completed7% completed8% completed9% completed10% completed11% completed12% completed13% completed14% completed15% completed16% completed17% completed18% completed19% completed20% completed21% completed22% completed23% completed24% completed25% completed26% completed27% completed28% completed29% completed30% completed31% completed32% completed33% completed34% completed35% completed36% completed37% completed38% completed39% completed40% completed41% completed42% completed43% completed44% completed45% completed46% completed47% completed48% completed49% completed50% completed51% completed52% completed53% completed54% completed55% completed56% completed57% completed58% completed59% completed60% completed61% completed62% completed63% completed64% completed65% completed66% completed67% completed68% completed69% completed70% completed71% completed72% completed73% completed74% completed75% completed76% completed77% completed78% completed79% completed80% completed81% completed82% completed83% completed84% completed85% completed86% completed87% completed88% completed89% completed90% completed91% completed92% completed93% completed94% completed95% completed96% completed97% completed98% completed99% completed100% completedDone
fit_slow <- panstripe(sim_slow$pa, sim_slow$tree)
#> Running bootstrap...
#> 1% completed2% completed3% completed4% completed5% completed6% completed7% completed8% completed9% completed10% completed11% completed12% completed13% completed14% completed15% completed16% completed17% completed18% completed19% completed20% completed21% completed22% completed23% completed24% completed25% completed26% completed27% completed28% completed29% completed30% completed31% completed32% completed33% completed34% completed35% completed36% completed37% completed38% completed39% completed40% completed41% completed42% completed43% completed44% completed45% completed46% completed47% completed48% completed49% completed50% completed51% completed52% completed53% completed54% completed55% completed56% completed57% completed58% completed59% completed60% completed61% completed62% completed63% completed64% completed65% completed66% completed67% completed68% completed69% completed70% completed71% completed72% completed73% completed74% completed75% completed76% completed77% completed78% completed79% completed80% completed81% completed82% completed83% completed84% completed85% completed86% completed87% completed88% completed89% completed90% completed91% completed92% completed93% completed94% completed95% completed96% completed97% completed98% completed99% completed100% completedDone

# Compare the fits
result <- compare_pangenomes(fit_fast, fit_slow)
result$summary
#> # A tibble: 2 x 5
#>   term  estimate std.error statistic p.value
#>   <chr>    <dbl>     <dbl>     <dbl>   <dbl>
#> 1 tip     -0.493     0.179     -2.75 0.00593
#> 2 core    -0.626     0.285     -2.19 0.0282
```

## Open vs Closed

The definition of what constitutes an open or closed pangenome is
somewhat ambiguous. We prefer to consider whether there is evidence for
a temporal signal in the pattern of gene gain and loss. This prevents
annotation errors leading to misleading results.

After fitting a `panstripe` model the significance of the temporal
signal can be assessed by considering the coefficient and p-value of the
‘core’ term in the model. The uncertainty of this estimate can also be
investigated by looking at the bootstrap confidence intervals of the
core term.

Let’s simulate a ‘closed’ pangenome

``` r
sim_closed <- simulate_pan(rate = 0, ngenomes = 100)
fit_closed <- panstripe(sim_closed$pa, sim_closed$tree)
#> Running bootstrap...
#> 1% completed2% completed3% completed4% completed5% completed6% completed7% completed8% completed9% completed10% completed11% completed12% completed13% completed14% completed15% completed16% completed17% completed18% completed19% completed20% completed21% completed22% completed23% completed24% completed25% completed26% completed27% completed28% completed29% completed30% completed31% completed32% completed33% completed34% completed35% completed36% completed37% completed38% completed39% completed40% completed41% completed42% completed43% completed44% completed45% completed46% completed47% completed48% completed49% completed50% completed51% completed52% completed53% completed54% completed55% completed56% completed57% completed58% completed59% completed60% completed61% completed62% completed63% completed64% completed65% completed66% completed67% completed68% completed69% completed70% completed71% completed72% completed73% completed74% completed75% completed76% completed77% completed78% completed79% completed80% completed81% completed82% completed83% completed84% completed85% completed86% completed87% completed88% completed89% completed90% completed91% completed92% completed93% completed94% completed95% completed96% completed97% completed98% completed99% completed100% completedDone

fit_closed$summary
#>     term    estimate    std.error     statistic      p.value
#> 1    tip  1.38929608 3.339793e-01  4.159827e+00 3.184891e-05
#> 2   core -1.32271520 2.612488e+04 -5.063047e-05 9.999596e-01
#> 3 height -0.01156471 7.851487e-02 -1.472932e-01 8.829006e-01
#>   bootstrap CI (2.5%) bootstrap CI (97.5%)
#> 1           0.8242535             1.972132
#> 2          -7.1400962             1.800430
#> 3          -0.1494689             0.121948
```

The p-value indicates that there is not a significant association
between core branch lengths and gene gain/loss. This is typical of
species that undergo very little to no recombination such as *M.
tuberculosis*.

## Rate vs Size

While comparing the `core` parameter of the model identifies differences
in the association between branch lengths and gene gain and loss it does
not indicate whether this is driven by higher rates of recombination or
simply larger recombination events involving more genes.

The `panstripe` model allows these two scenarios to be investigated by
plotting the estimated parameters from the underlying Tweedie model. The
`Poisson` like part of the model indicates the rate of recombination
events while the `Gamma` like part of the model indicates the typical
size of the events.

Here, we simulate two data sets with the same recombination rate but
where each recombination event differs in the number of genes involved.
The `plot_dist_params` can then be used to compare these two datasets.

``` r
sim_large <- simulate_pan(rate = 0.001, ngenomes = 100, mean_trans_size = 10)
sim_small <- simulate_pan(rate = 0.001, ngenomes = 100, mean_trans_size = 2)

fit_large <- panstripe(sim_large$pa, sim_large$tree)
#> Running bootstrap...
#> 1% completed2% completed3% completed4% completed5% completed6% completed7% completed8% completed9% completed10% completed11% completed12% completed13% completed14% completed15% completed16% completed17% completed18% completed19% completed20% completed21% completed22% completed23% completed24% completed25% completed26% completed27% completed28% completed29% completed30% completed31% completed32% completed33% completed34% completed35% completed36% completed37% completed38% completed39% completed40% completed41% completed42% completed43% completed44% completed45% completed46% completed47% completed48% completed49% completed50% completed51% completed52% completed53% completed54% completed55% completed56% completed57% completed58% completed59% completed60% completed61% completed62% completed63% completed64% completed65% completed66% completed67% completed68% completed69% completed70% completed71% completed72% completed73% completed74% completed75% completed76% completed77% completed78% completed79% completed80% completed81% completed82% completed83% completed84% completed85% completed86% completed87% completed88% completed89% completed90% completed91% completed92% completed93% completed94% completed95% completed96% completed97% completed98% completed99% completed100% completedDone
fit_small <- panstripe(sim_small$pa, sim_small$tree)
#> Running bootstrap...
#> 1% completed2% completed3% completed4% completed5% completed6% completed7% completed8% completed9% completed10% completed11% completed12% completed13% completed14% completed15% completed16% completed17% completed18% completed19% completed20% completed21% completed22% completed23% completed24% completed25% completed26% completed27% completed28% completed29% completed30% completed31% completed32% completed33% completed34% completed35% completed36% completed37% completed38% completed39% completed40% completed41% completed42% completed43% completed44% completed45% completed46% completed47% completed48% completed49% completed50% completed51% completed52% completed53% completed54% completed55% completed56% completed57% completed58% completed59% completed60% completed61% completed62% completed63% completed64% completed65% completed66% completed67% completed68% completed69% completed70% completed71% completed72% completed73% completed74% completed75% completed76% completed77% completed78% completed79% completed80% completed81% completed82% completed83% completed84% completed85% completed86% completed87% completed88% completed89% completed90% completed91% completed92% completed93% completed94% completed95% completed96% completed97% completed98% completed99% completed100% completedDone

plot_dist_params(list(large = fit_large, small = fit_small))
```

![](inst/vignette-supp/unnamed-chunk-7-1.png)<!-- -->

Similarly, we can compare the previous fits of fast and slow
recombination rates where the mean size of the recombination events
remains the same.

``` r
plot_dist_params(list(fast = fit_fast, slow = fit_slow))
```

![](inst/vignette-supp/unnamed-chunk-8-1.png)<!-- -->

## Plots

Panstripe includes a number of useful plotting functions to help with
interpretation of the output of
[panaroo](https://gtonkinhill.github.io/panaroo/#/).

### Pangenome fit

A simple plot of the fit of the model along with the input data points
can be made by running

``` r
plot_pangenome_fits(fit_fast)
```

![](inst/vignette-supp/unnamed-chunk-9-1.png)<!-- -->

The `plot_pangenome_fits` function can also take a named list as input
allowing for easy comparisons between data sets

``` r
plot_pangenome_fits(list(fast = fit_fast, slow = fit_slow))
```

![](inst/vignette-supp/unnamed-chunk-10-1.png)<!-- -->

### Tree with presence/absence

A plot of the phylogeny and the corresponding gene presence/absence
matrix can be made using the `plot_tree_pa` function. This function also
takes an optional vector of gene names to include in the plot.

``` r
# look at only those genes that vary
variable_genes <- colnames(pa)[apply(pa, 2, sd) > 0]

plot_tree_pa(tree = tree, pa = pa, genes = variable_genes, label_genes = FALSE, cols = "black")
```

![](inst/vignette-supp/unnamed-chunk-11-1.png)<!-- -->

### Inferred ancestral states

The `plot_gain_loss` function allows for the visualisation of the fitted
gene gain and loss events on the given phylogeny. The enrichment for
events at the tips of a tree is often driven by a combination of highly
mobile elements and annotation errors.

``` r
plot_gain_loss(fit)
```

![](inst/vignette-supp/unnamed-chunk-12-1.png)<!-- -->

### tSNE

The tSNE dimension reduction technique can be used to investigate
evidence for clusters within the pangenome.

``` r
plot_tsne(pa)
```

![](inst/vignette-supp/unnamed-chunk-13-1.png)<!-- -->

The [Mandrake](https://github.com/johnlees/mandrake) method can also be
used as an alternative to tSNE.

### Accumulation curves

While we do not recommend the use of accumulation curves as they do not
account for population structure, sampling bias or annotation errors we
have included a function to plot them to make it easier for users to
compare methods.

``` r
plot_acc(list(fast = sim_fast$pa, slow = sim_slow$pa))
```

![](inst/vignette-supp/unnamed-chunk-14-1.png)<!-- -->
