# QBRI-Seq

C++-based NGS Pipeline wrapper for WES and RNA-Seq created for the Qatar Biomedical Research Institute. This project produces two executables when compiled - __qbri-wes__ and __qbri-rna__.

An overview of the pipeline can be found in the pdf documents.



__N.B.__ New installations will require a *__config.h__* file creating prior to installation. To do so you should copy *config.h.bak* to *config.h*, then edit this new file to set the variables to suit your specific system. However, please note that PICARDMEM should not exceed 30GB.
