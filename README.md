# GraphColorContraction

Graph contraction on attribute-based coloring

Networks play a ubiquitous role in computer science and real-world applications.
Using distinctive node features to group such data into clusters, hence represented by a single representative node per cluster, proves to be a valuable approach.
These contracted graphs can reveal previously hidden characteristics of the original networks.

In many real-world cases, clusters can be identified by a set of connected vertices that share the result of some categorical function, _i.e._ a mapping of the vertices into some categorical representation that takes values in a finite set _C_.
As an example, we can identify contiguous terrains with the same discrete property on a geographical map (leveraging Space Syntax) or contiguous nodes having the same discrete property in a social network.
Contracting a graph allows a more scalable analysis of the interactions and structure of the network nodes. 

More details can be found in:
 - [![DOI](https://zenodo.org/badge/doi/10.5383/JUSPN.17.02.006.svg)](http://doi.org/10.5383/JUSPN.17.02.006) [Some Results on Colored Network Contraction, JUSPN 17 (2) pp.91-98, Dec 2022](https://iasks.org/articles/juspn-v17-i2-pp-91-98.pdf)
 - [![DOI](https://zenodo.org/badge/doi/10.1016/j.procs.2022.03.056.svg)](http://doi.org/10.1016/j.procs.2022.03.056) [Graph Contraction on Attribute-Based Coloring, Procedia Computer Sciences 201 pp.429-436, Apr 2022](https://doi.org/10.1016/j.procs.2022.03.056)

## Description

The present repository contains the C implementation of our novel algorithm for graph contraction over attribute-based coloured graphs and can be used to test the algorithm on any undirected graph equipped with a colouring. The output of the algorithm consists in a novel graph where all the adjacent vertices sharing the same colour are contracted together. This approach allows extracting a simplified version of a graph while preserving interesting connectivity properties.

## Structure of the repository

 - `app/` where the compiled executables are placed
 - `bin/` where the compiled auxiliary files are placed
 - `config/` configuration files directory
 - `data/` basedir for graph location
   - `generated/` where Erdos-Renyi random graphs are generated
 - `logs/` where logs are saved
 - `results/` basedir where the output of the contraction is stored
 - `src/` source code of the auxiliary files
 - `target/` source code of the executables

## Usage

Before using this repository, run `make install` to set up the configuration files.
In the current version, a single file (config/serial.number) is created;
this file is used by random graph generator to avoid overriding the same file when a new generation is raised.

Executing `make help` (or simply `make`) displays basic information about which operations can be performed and about the compile settings.

The targets contained in the `target/` directory can be compiled by simply typing `make <target>` and, once compiled, can be run with `./app/<target>`.
To know how to use a target and which optional parameters are available, simply use `./app/<target> -?` or `./app/<target> --help`.

To remove the compiled sources use `make clean` and to remove the contraction outputs use `make data-cleanup`.
Do note that generated graphs with `genErdosRenyi` target will not be removed this way.

Compilation for debug (`-g -O0 -Wall`) and for profiling (`-pg -O2`) can be enabled by calling `make DEBUG=true <target>` or `make PROFILE=true <target>`.

## Authors and acknowledgment

[ORCID:0000-0001-8391-2563](https://orcid.org/0000-0001-8391-2563)\
[Elia Onofri](https://sites.google.com/view/elia-onofri/home)
obtained the bachelor's degree in Mathematics in 2018 and the master's degree in Computational Sciences in 2020 at Roma Tre University, Rome, Italy. Currently, he is a Ph.D. student in Mathematics at Roma Tre University, under a collaboration with the National Research Council of Italy where he do has a research fellowship.
His research fields concern applied mathematics in cryptography, graph theory, machine learning, biology and forecasting simulations.

[ORCID:0000-0003-0723-7847](https://orcid.org/0000-0003-0723-7847)\
[Flavio Lombardi](http://ricerca.mat.uniroma3.it/users/lombardi/) is a researcher at The Istituto per le Applicazioni del Calcolo of the National Research Council of Italy (IAC-CNR).

## How to cite

```bib
@article{LombardiOnofri22a,
	author = {Lombardi, Flavio and Onofri, Elia},
	doi = {10.1016/j.procs.2022.03.056},
	issn = {1877-0509},
	journal = {Procedia Computer Science},
	month = {04},
	note = {The 13th International Conference on Ambient Systems, Networks and Technologies (ANT) / The 5th International Conference on Emerging Data and Industry 4.0 (EDI40)},
	pages = {429--436},
	title = {Graph contraction on attribute-based coloring},
	volume = {201},
	year = {2022}
}

@article{LombardiOnofri22b,
	author = {Lombardi, Flavio and Onofri, Elia},
	day = {20},
	doi = {10.5383/JUSPN.17.02.006},
	journal = {Journal of Ubiquitous Systems and Pervasive Networks},
	month = {12},
	number = {2},
	pages = {91--98},
	title = {Some results on colored network contraction},
	volume = {17},
	year = {2022}
}
```

## How to contribute

Fork this repo and send a pull request to [Elia Onofri](https://github.com/eOnofri04)