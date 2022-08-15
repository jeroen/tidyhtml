#include <tidy.h>
#include <tidybuffio.h>
#include <Rinternals.h>

SEXP C_tidy_html(SEXP str, SEXP filename) {
  const char* input = CHAR(Rf_asChar(str));
  const char* name = CHAR(Rf_asChar(filename));
  TidyBuffer outbuf = {0};
  TidyBuffer errbuf = {0};
  TidyDoc tdoc = tidyCreate();
  if(!tdoc)
    Rf_error("Failed to initiate tidyhtml");

  /* Parsable output */
  tidyOptSetBool(tdoc, TidyEmacs, yes );
  tidyOptSetValue(tdoc, TidyEmacsFile, name);

  /* Parse input string */
  tidySetErrorBuffer( tdoc, &errbuf );
  if(tidyParseString( tdoc, input) > 1){
    Rf_error("Failed to parse input %s", name);
  }

  /* Clean and print warnings */
  tidyCleanAndRepair( tdoc );
  tidyRunDiagnostics( tdoc );

  /* Store output */
  tidyOptSetBool(tdoc, TidyForceOutput, yes);
  tidySaveBuffer( tdoc, &outbuf );

  /* Store and free */
  SEXP output = PROTECT(Rf_allocVector(STRSXP, 2));
  SET_STRING_ELT(output, 0, Rf_mkCharLenCE((char*) errbuf.bp, errbuf.size, CE_UTF8));
  SET_STRING_ELT(output, 1, Rf_mkCharLenCE((char*) outbuf.bp, outbuf.size, CE_UTF8));
  tidyBufFree( &outbuf );
  tidyBufFree( &errbuf );
  tidyRelease( tdoc );
  UNPROTECT(1);
  return output;
}
