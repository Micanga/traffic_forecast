library("forecast")
source('utils.R')
# ADAPTFLOW FORECASTING PACKAGE .................
createCoef <- function(){
	coef = list(day = c())
    for(i in 1:1){
        coef$day = c(coef$day,list(hour = c()))
        for(j in START_PERIOD:END_PERIOD){
	    	coef$day[i]$hour = c(coef$day[i]$hour,1.0)
        }
    }
    return(coef)
}


weightedtraining <- function(filename,ntrainning_days,weight){
	expected_train = ntrainning_days
	file = read.csv(filename)
    #1. Creating and initializing the support structures to make the training
    coef = createCoef()
    index = 1
        
    ntests = 0
    for( day in START_DAY:END_DAY){
    	#2. Vectorizing day flow
    	flow <- c()
		for( i in (START_PERIOD+(END_PERIOD*(day-1))):(END_PERIOD*day))
			flow <- c(flow,file[i,1])

        ntests = ntests + 1
                    
        Erro[index,1] = 0
        coef$day[index]$hour[1] = 1
        
        #3. TRAINING
        for(hour in (START_PERIOD+1):END_PERIOD){
            if(ntests == 1)
                bestCoef[index,hour] = 1+(flow[hour]-flow[hour-1])/flow[hour-1]
            else
	            bestCoef[index,hour] = weight*(1+(flow[hour]-flow[hour-1])/flow[hour-1]) + (1 - weight)*coef$day[index]$hour[hour]
                       
            coef$day[index]$hour[hour] = bestCoef[index,hour]
        }
        ntrainning_days = ntrainning_days - 1
    }

    #3. Validating the training
    if(ntests != expected_train)
    	return(NULL)

    return(coef)
}

adaptFlow <- function(filename,ntraining_days,npredictions,weight,npoints,dweight){
	file = read.csv(filename)
	#1. Calculating the coef
	coef = weightedtraining(filename,ntraining_days,weight)
	#print(coef)

	#2. Creating the hour vector (used in lsqr method)
    hour_vec = c()
    for(hour in START_PERIOD:END_PERIOD)
        hour_vec = c(hour_vec,hour)

    #3. Starting the variables and aux vectors
    index = 1
    day = 1

    f = c()
	for( i in START_PERIOD:(END_PERIOD*4) )
		f = c(f,as.numeric(file[i,1]))
    
    pf = c()
    for( i in START_PERIOD:END_PERIOD[])
		pf = c(pf,0.0)

	dtree = matrix(0.0,N_WEEK_DAYS,npoints)

    #4. initializing the variables
	Error = 0
	TotalFlow = 0
	errors = c()
	for( i in START_PERIOD:END_PERIOD)
		errors = c(errors,0.0)
            
	#5. running the test
	hour = START_PERIOD
	for( hour in START_PERIOD:(END_PERIOD-npredictions)){
		TotalFlow = TotalFlow + f[hour+npredictions-1]
        expected_result = f[hour+npredictions-1]

		if(hour != START_PERIOD){
            pf[hour-1] = f[hour-1]
            
            for(phour in 0:(npredictions-1)){
                if(phour+hour >= END_PERIOD)
                    break

        		#a. applying the least squares method to short term prediction
        		x <- c()
        		y <- c()
        		if(hour+phour == 2){
        			ls_next = as.numeric(pf[hour-1])
        		}
        		else if(hour+phour < npoints){
        			x = hour_vec[1:(hour+phour-1)]
        			y = pf[1:(hour+phour-1)]
        			fit = lsfit(x,y)
        			ls_next = (as.numeric(fit$coefficients[2])*(hour+phour))+as.numeric(fit$coefficients[1])
        		}
                else if(npoints == 0 || hour+phour == 24){
                    ls_next = as.numeric(pf[hour-1])	
                }
        		else{
        			x = hour_vec[(hour+phour-3):(hour+phour-1)]
        			y = pf[(hour+phour-3):(hour+phour-1)]
        			fit = lsfit(x,y)
        			ls_next = (as.numeric(fit$coefficients[2])*(hour+phour))+as.numeric(fit$coefficients[1])
        		}
                            
                #b. Calculating the forecasting point and choosing between the dynamic or static approach
                if(phour > 0 && hour > npoints){
                    expectedbehavior = 0
                    for(i in 1:npoints)
                        expectedbehavior = expectedbehavior + (dtree[index,i] >= 0.5)?(1):(-1)

                    if(expectedbehavior >= 0 && dweight < 0.5)
                        dweight = 1-dweight
                    else if(expectedbehavior < 0 && dweight >= 0.5)
                        dweight = 1-dweight

                    dweight = coef->day[index]$coef[hour+phour]*dweight
                    if(dweight > 1)
                        dweight = 1

                    for( i in 1:npoints)
                        dtree[index,npoints-i] = dtree[index,npoints-i-1]
                    dtree[index,1] = dweight
                }
                F = (dweight)*(as.numeric(pf[hour+phour-1])*as.numeric(coef$day[index]$hour[hour+phour])) + (1 - dweight)*(ls_next)

        		#c. Updating the prediction flows vector
                pf[hour+phour] = F
            }

            #d. Calculating the errors
            weigError = abs(expected_result - f[hour-1]*coef$day[index]$hour[hour+npredictions-1])
            lsqrError = abs(expected_result - ls_next)

            #e. Updating the dtree values
            for( i in 1:npoints)
                dtree[index,npoints-i] = dtree[index,npoints-i]
            dtree[index,1] = dweight

            #f. Updating the weight value to improove next prediction
            totalError = weigError + lsqrError
            if(weigError > lsqrError)
                dweight = 1 - (weigError/totalError)
            else
                dweight = (lsqrError/totalError)

            #g. Updating the hour 
            Error = Error + abs(expected_result - F)
            errors[hour+phour] = abs(expected_result - F)
		}    
  		else{
            errors[hour] = 0
        }
			    
	}

    #6. priting the errors in outputfile
    #print(100*Error/TotalFlow)
	#write(c(100*Error/TotalFlow),file=outputfilename("AdaptFlow",npredictions),ncolumns = 1,append = TRUE,sep = "")

	return(TRUE)
}

runtestAdaptFlow <- function(npredictions,filename){
	#1. Verifying the file existence
	if(file.exists(filename)){
		print(paste("Start test - file :",filename))
		sucess_flag = adaptFlow(filename,N_TRAINING_DAYS,npredictions,WEIGHT,N_POINTS,DWEIGHT)
	}
}
