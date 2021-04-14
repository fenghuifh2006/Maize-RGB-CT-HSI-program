### blup estimates for phenotypic traits across multiple envionments##
### Broad-sense heritability estimation across multiple envionments
### THE DATA SHOULD INCLUDE THREE COLUMNS AT LEAST,DENOTED AS 'LINE','LOC' AND 'TRAIT1...TRAITn'



Onewaybluph2NEW<-function(x){
##Calculate variance components
##require package lme4
library(Matrix)
library(lme4)
id<-as.character(x$LINE[x$LOC%in%unique(x$LOC)[1]])
line.blup<-list()
heritability<-list()

for(i in 3:ncol(x)){
		cat(names(x)[i],'start\n')
		varcomp<-lmer(x[,i]~(1|LINE)+(1|LOC),data=x) 
		vc = data.frame(VarCorr(varcomp))
		var = vc[match(c('LINE','LOC','Residual'),vc$grp),c('grp','vcov')]
		names(var)=c('Groups','Variance')
		gvar<-as.numeric(as.character(var$Variance))[var$Groups%in%'LINE']
		evar<-as.numeric(as.character(var$Variance))[var$Groups%in%'Residual']
		heritability[[i-2]]<-matrix(gvar/(gvar+evar/length(unique(x$LOC))),1,1,dimnames=list(colnames(x)[i],'H2'))
		f<-fixef(varcomp)
		r<-ranef(varcomp)$LINE
		blup<-round(f+r,4)
		line.blup[[i-2]]<-blup[match(id,rownames(blup)),]
	}
	heritability=do.call('rbind',heritability)
	line.blup=data.frame(id,do.call('cbind',line.blup))
	colnames(line.blup)<-c('line',names(x)[-c(1:2)])

	cat('----OUTPUT RESULTS OF EACH TRAITS INTO FILES:----','\n','----BroadSenseHeritability.txt----','\n',
 '----BlupEachlines.txt----','\n')
 
	write.table(heritability,'RGB-CK_BroadSenseHeritability.txt',sep='\t',quote=F)   
	write.csv(line.blup,'RGB-CK_BlupEachlines.csv',row.names=F)    
}


x<-read.table("RGB-CK.txt",header=T,sep="\t")   

Onewaybluph2NEW(x)


