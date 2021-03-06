#!/usr/bin/env Rscript
if (!("VineCopula" %in% rownames(installed.packages()))) {
     install.packages("VineCopula", repos="http://cran.rstudio.com/")
}

set.seed(5)
mat <- matrix(c(4, 0, 0, 0, 0, 0, 0,
                7, 3, 0, 0, 0, 0, 0,
                3, 7, 7, 0, 0, 0, 0,
                1, 1, 5, 1, 0, 0, 0,
                2, 5, 2, 5, 2, 0, 0,
                6, 6, 1, 2, 5, 5, 0,
                5, 2, 6, 6, 6, 6, 6),
              7, 7, byrow = TRUE)
fam <- par <- matrix(0, 7, 7)
fam[lower.tri(fam)] <- 23
par[lower.tri(par)] <- -3
model <- VineCopula::RVineMatrix(mat, fam, par)
u <- VineCopula::RVineSim(1000, model)
fit <- VineCopula::RVineStructureSelect(u, familyset = 0)

write.table(cbind(u, 
                  VineCopula::RVinePDF(u, model),
                  VineCopula::RVineSim(1000, model, U = u)),
            file = "temp", col.names = FALSE, row.names = FALSE)
write.table(mat, file = "temp2", col.names = FALSE, row.names = FALSE)
write.table(fit$Matrix, file = "temp3", col.names = FALSE, row.names = FALSE)