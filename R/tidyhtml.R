#' Tidy up HTML
#'
#' Cleans up HTML and also returns a list of warnings.
#'
#' @export
#' @param file path
#' @useDynLib tidyhtml C_tidy_html
tidy_html <- function(file){
  file <- normalizePath(file, mustWork = TRUE)
  str <- rawToChar(readBin(file, raw(), file.info(file)$size))
  out <- .Call(C_tidy_html, str, basename(file))
  list(
    diagnostics = out[1],
    html = out[2]
  )
}
