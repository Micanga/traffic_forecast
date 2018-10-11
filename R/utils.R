# GLOBAL VARIABLES ..................
N_TRAINING_DAYS <<- 3
N_POINTS <<- 5
WEIGHT <<- 0.60
DWEIGHT <<- 0.85
START_PERIOD <<- 1
END_PERIOD <<- 24
N_WEEK_DAYS <<- 7
START_DAY <<- 1
END_DAY <<- 30

# FORECASTING UTILS PACKAGE ....................
e_month <- function() {
    list(JAN = 1, FEB = 2, MAR = 3, APR = 4, MAY = 5, JUN = 6, JUL = 7, AUG = 8, SET = 9, OUT = 10, NOV = 11, DEC = 12)
}

e_dayofweek <- function() {
    list(SUN = 1, MON = 2, TUE = 3, WED = 4, THU = 5, FRI = 6, SAT = 7)
}

isfeb <- function(month){
	if(month == e_month()$FEB)
		return(TRUE)
	return(FALSE)
}

outputfilename <- function(method,npredictions){
	name <- paste("","",collapse = "",sep = "")
	    
	name <- paste("Error",method,collapse = "",sep = "")
	name <- paste(name,as.character(npredictions),collapse = "",sep = "")

	return(name)
}

testfilename <- function(month,link_ref){
	name <- paste("","",collapse = "",sep = "")

        if(month == 1){
	    name <- paste("all_data/",link_ref,collapse = "",sep = "")
	    name <- paste(name,"Jan",collapse = "",sep = "")
	}
	else if(month == 2){
	    name <- paste("all_data/",link_ref,collapse = "",sep = "")
	    name <- paste(name,"Feb",collapse = "",sep = "")
	}
	else{
	     name <- paste("all_data/",link_ref,collapse = "",sep = "")
	     name <- paste(name,"Mar",collapse = "",sep = "")
        }
	
	return(name)
}

msdtrainning <- function(test_day, test_month, ntraining_days, month){
    startday = test_day
    while(ntraining_days > 0){
    	ntraining_days = ntraining_days-1

        startday = startday - 7
        if(startday <= 0){
			if(month == e_month()$FEB) startday = startday + 28
			else if(month %% 2 == 1) startday = startday + 31
			else startday = startday + 30

            if(test_month == month) return(1)
            else test_month = if(test_month == 0) e_month()$DEC else test_month-1
        }
    }
    if(test_month != month) return(35)
    else return(startday)
}