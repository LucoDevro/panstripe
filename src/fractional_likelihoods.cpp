// This function has been simplified and adapted from the sfreemap R package (https://bmcbioinformatics.biomedcentral.com/articles/10.1186/s12859-017-1554-7)

#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
List fractional_likelihoods(NumericMatrix edges, 
                             NumericMatrix states,
                             NumericVector prior,
                             List tp
) {
  
  // Initialise variables
  int e, i, j, r, l, p;
  int n_edges = tp.size();
  int n_states = edges.ncol();
  int n_nodes = n_edges+1;
  
  NumericMatrix g(n_nodes, n_states);
  NumericMatrix s(n_nodes, n_states);
  NumericMatrix f(n_nodes, n_states);
  
  // Initialize f
  for (i=0; i<states.nrow(); i++) {
    f.row(i) = states.row(i);
  }
  
  // calculate f and s
  for (e=0; e<n_edges; e+=2){
    p = edges(e,0) - 1;
    r = edges(e,1) - 1;
    l = edges(e+1,1) - 1;
    
    NumericMatrix rm = tp[e];
    NumericMatrix lm = tp[e+1];
    
    for (i=0; i<n_states; i++){
      for (j=0; j<n_states; j++){
        s(r,i) += f(r,j) * rm(i,j);
        s(l,i) += f(l,j) * lm(i,j);
      }
      f(p,i) = s(r,i) * s(l,i);
    }
  }
  
  // calculate the likelihood
  int root_node = edges(n_edges-1,0)-1;
  double likelihood = 0;
  for (i=0; i<n_states; i++){
    likelihood += f(root_node, i) * prior(i);
  }
  g.row(root_node) = prior;
  
  // calculate g
  for (e=n_edges-1; e>=1; e-=2){
    p = edges(e,0) - 1;
    r = edges(e-1,1) - 1;
    l = edges(e,1) - 1;
    
    NumericMatrix rm = tp[e-1];
    NumericMatrix lm = tp[e];
    
    for (i=0; i<n_states; i++){
      for (j=0; j<n_states; j++){
        g(l,i) += g(p,j) * s(r,j) * lm(i,j);
        g(r,i) += g(p,j) * s(l,j) * rm(i,j);
      }
    }
  }
  
  return List::create(Named("F") = f,
                      Named("S") = s,
                      Named("G") = g,
                      Named("L") = likelihood);
}



/*** R
# check things are working
edges <- matrix(c(13,13,18,18,19,19,12,12,17,17,16,16,15,15,14,14,11,11,1,2,4,5,8,9,13,3,18,6,17,7,16,19,15,10,12,14), ncol = 2)
states <- matrix(c(1,0,1,1,0,0,1,0,1,0,0,1,0,0,1,1,0,1,0,1), ncol = 2)
prior <- c(0.5,0.5)
tp <- array(c(0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.63,0.37,0.37,0.63,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.69,0.31,0.31,0.69), c(2,2,18))
tp <- lapply(1:18, function(i) tp[,,i])

out <- fractional_likelihoods(edges, states, prior, tp)
stopifnot(round(out$likelihood,6)==0.000977)
*/
