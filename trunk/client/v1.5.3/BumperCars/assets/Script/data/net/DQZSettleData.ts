//夺旗战游戏结算数据
export default class DQZSettleData{

    private _redScore: number   = null;     //红方总积分
    private _blueScore: number  = null;     //蓝方总积分

    private _redGoldNum: number = null;     //红方金币数量
    private _blueGoldNum: number= null;     //蓝方金币数量

    private _redResutl: number  = null;     //红方战斗结果
    private _blueResult: number = null;     //蓝方战斗结果

    private _doubleGold: number = null;     //翻倍后的奖励

    constructor()
    {
        this._redScore = 0;
        this._blueScore = 0;
        this._redGoldNum = 0;
        this._blueGoldNum = 0;
        this._redResutl = 0;
        this._blueResult = 0;
        this._doubleGold = 0;
    }

    setDoubleGold(goldNum: number)
    {
        this._doubleGold = goldNum;
    }

    getDoubleGold(): number
    {
        return this._doubleGold;
    }

    setRedGoldNum(goldNum: number)
    {
        this._redGoldNum = goldNum;
    }

    setBlueGoldNum(goldNum: number)
    {
        this._blueGoldNum = goldNum;
    }

    getRedGoldNum(): number
    {
        return this._redGoldNum;
    }

    getBlueGoldNum(): number
    {
        return this._blueGoldNum;
    }

    setRedResult(result: number)
    {
        this._redResutl = result;
    }

    getRedResult(): number
    {
        return this._redResutl;
    }
    
    setBlueResult(result: number)
    {
        this._blueResult = result;
    }

    getBlueResult(): number
    {
        return this._blueResult;
    }

    setRedScore(score: number)
    {
        this._redScore = score;
    }

    getRedScore(): number
    {
        return this._redScore;
    }

    setBlueScore(score: number)
    {
        this._blueScore = score;
    }

    getBlueScore(): number
    {
        return this._blueScore;
    }
}