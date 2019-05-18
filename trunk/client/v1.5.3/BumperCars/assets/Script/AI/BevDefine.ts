export enum BevState{
	BRS_Executing = 0,
	BRS_Finish = 1,
};

export enum BevTerminalState{
	BTS_READY = 0,
	BTS_RUNNING = 1,
	BTS_FINISH= 2,
};

export const BRS_ERROR_TRANSITION =  -1;

export const MAX_CHILDREN =  5;