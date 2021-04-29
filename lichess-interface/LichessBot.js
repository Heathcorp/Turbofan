const fs = require("fs");
const fetch = require('node-fetch');
const readline = require('readline');

const personalToken = JSON.parse(fs.readFileSync("auth.json")).token; //secret lichess api token, token.json is ignored by git
const botPlayerId = "turbofan";

const maxGames = 1;

var currentGames = [];
var currentChallenges = [];

//to-do: fix challenge acceptance and game management, make engine

Main();

function Main() {
    fetch('https://lichess.org/api/stream/event', {
        method: "get",
        headers: {
            'Authorization': 'Bearer ' + personalToken
        }
    }).then(response => {
        const responseReader = readline.createInterface({input: response.body});
        responseReader.on("line", line => {
            if (line != "") {
                let lichessEvent = JSON.parse(line);

                if (lichessEvent.type == "challenge") {
                    //fix this
                    AcceptChallenge(lichessEvent.challenge.id);
                    //currentChallenges[lichessEvent.challenge.id] = lichessEvent.challenge;
                } else if (lichessEvent.type == "challengeCanceled" || lichessEvent.type == "challengeDeclined") {
                    delete currentChallenges[lichessEvent.challenge.id];
                } else if (lichessEvent.type == "gameStart") {
                    currentGames[lichessEvent.game.id] = lichessEvent.game;
                    PlayGame(lichessEvent.game.id);
                } else if (lichessEvent.type == "gameFinish") {
                    delete currentGames[lichessEvent.game.id];
                }
            }
        });
    }).catch(error => {
        console.log(error);
    });
}

function PlayGame(gameId)
{
    let gameState = {
        isWhite: true,
        opponent: "",
        initialState: "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", //FEN
        moves: [],
        timeLeft: 0,
        result: "unfinished" //either "unfinished", "white", "black", "aborted" or "draw"
    };

    //create new instance of the engine

    fetch("https://lichess.org/api/bot/game/stream/" + gameId, {
        method: "get",
        headers: {
            'Authorization': 'Bearer ' + personalToken
        }
    }).then(response => {
        const responseReader = readline.createInterface({input: response.body});
        responseReader.on("line", line => {
            if (line != "") {
                let boardEvent = JSON.parse(line);
                if (boardEvent.type == "chatLine") {
                    //maybe make a super toxic chatbot
                } else {
                    if (boardEvent.type == "gameFull") {
                        gameState.isWhite = (boardEvent.white.id == botPlayerId);
                        gameState.opponent = gameState.isWhite ? boardEvent.black.name : boardEvent.white.name;
                        if (boardEvent.gameState != "startpos") {
                            gameState.initialState = boardEvent.initialFen;
                        }
                        gameState.moves = boardEvent.state.moves.split(" ");
                        console.log("Playing game " + gameId + " as " + (gameState.isWhite ? "white" : "black") + " against " + gameState.opponent);
                    } else if (boardEvent.type == "gameState") {
                        gameState.moves = boardEvent.moves.split(" ");
                        gameState.timeLeft = gameState.isWhite * boardEvent.wtime + !gameState.isWhite * boardEvent.btime;
                        if (boardEvent.winner == "white") {
                            gameState.result = "white";
                        } else if (boardEvent.winner == "black") {
                            gameState.result = "black";
                        } else if (boardEvent.status == "aborted") {
                            gameState.result = "aborted";
                        } else if (boardEvent.status == "draw") {
                            gameState.result = "draw";
                        }
                        
                    }
                    if (gameState.result == "unfinished") {
                        if (gameState.isWhite && ((gameState.moves.length % 2) == 0) || !gameState.isWhite && ((gameState.moves.length % 2) == 1)) {
                            //other player moved, feed move into engine instance, play move upon promise resolution
                        }
                    } else {
                        if ((gameState.result == "white" && gameState.isWhite) || (gameState.result == "black" && !gameState.isWhite)) {
                            console.log("Won game " + gameId + " (" + boardEvent.status + ")");
                        } else if ((gameState.result == "white" && !gameState.isWhite) || (gameState.result == "black" && gameState.isWhite)) {
                            console.log("Lost game " + gameId + " (" + boardEvent.status + ")");
                        } else if (gameState.result == "aborted") {
                            console.log("Game " + gameId + " aborted");
                        } else if (gameState.result == "draw") {
                            console.log("Drew game " + gameId);
                        }
                        //game over, close/repurpose engine instance and close game stream, maybe say something toxic in chat
                    }
                }
            }
        });
    }).catch(error => {
        console.log(error);
    });
}

//this function isn't necessary because the lichess event stream sends all active games when the stream opens
function GetGames() {
    fetch('https://lichess.org/api/account/playing', {
        method: "get",
        headers: {
            'Authorization': 'Bearer ' + personalToken
        }
    }).then(response => response.json()).then(response => {
        currentGames = Array(response.nowPlaying.length);
        for (i = 0; i < response.nowPlaying.length; i++) {
            currentGames[i] = response.nowPlaying[i];
        }
    }).catch(error => {
        console.log(error);
    });
}

function MakeMove(gameId, move) {
    //returns a promise that resolves whether the move played successfully or not
    return new Promise((resolve, reject) => 
        fetch('https://lichess.org/api/bot/game/' + gameId + "/move/" + move, {
            method: "post",
            headers: {
                'Authorization': 'Bearer ' + personalToken
            }
        }).then(response => response.json()).then(response => {
            if (response.ok != undefined) {
                console.log("Played " + move + " in game " + gameId);
                resolve(true);
            } else {
                console.log("Failed to play " + move + " in game " + gameId + " (" + response.error + ")");
                resolve(false);
            }
        }).catch(error => {
            console.log(error);
            reject(error);
        })
    );
}

function AcceptChallenge(challengeId) {
    fetch('https://lichess.org/api/challenge/' + challengeId + "/accept", {
        method: "post",
        headers: {
            'Authorization': 'Bearer ' + personalToken
        }
    }).catch(error => {
        console.log(error);
        reject(error);
    });
}