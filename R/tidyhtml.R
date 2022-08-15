#' Tidy up HTML
#'
#' Cleans up HTML and also returns a list of warnings.
#'
#' @export
#' @rdname tidy_html
#' @param file path to input file
#' @useDynLib tidyhtml C_tidy_html
tidy_html_file <- function(file){
  file <- normalizePath(file, mustWork = TRUE)
  str <- rawToChar(readBin(file, raw(), file.info(file)$size))
  tidy_html_string(str, basename(file))
}

#' @export
#' @rdname tidy_html
#' @param filename (dummy) name to show in output
tidy_html_string <- function(str, filename = ""){
  out <- .Call(C_tidy_html, str, filename)
  structure(as.list(out), names = c('diagnostics', 'html'))
}

#' @export
#' @rdname tidy_html
#' @param filename (dummy) name to show in output
#' @param ... passed to `tools::Rd2HTML`
tidy_html_rd <- function(file, ...){
  file <- normalizePath(file, mustWork = TRUE)
  out <- file.path(tempdir(), paste0(basename(file), '.html'))
  on.exit(unlink(out))
  tools::Rd2HTML(file, out = out, ...)
  tidy_html_file(out)
}
