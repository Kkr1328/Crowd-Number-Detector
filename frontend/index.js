import { initializeApp } from "https://www.gstatic.com/firebasejs/9.8.1/firebase-app.js";
import { getDatabase, ref, onValue} from "https://www.gstatic.com/firebasejs/9.8.1/firebase-database.js";

const firebaseConfig = {
  apiKey: "AIzaSyBWx-PvSeVtloXpZHdOtdbojo35Hq6grS4",
  authDomain: "limit-people-19e80.firebaseapp.com",
  databaseURL: "https://limit-people-19e80-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "limit-people-19e80",
  storageBucket: "limit-people-19e80.appspot.com",
  messagingSenderId: "319024013208",
  appId: "1:319024013208:web:a7f17522fe14584952d5ed"
};
  
const app = initializeApp(firebaseConfig);

const db = getDatabase();

const numCountRef = ref(db, 'count');
onValue(numCountRef, (snapshot) => {
  const data = parseInt(snapshot.val());
  const limitNum = 5; // change limit number here
  if (data > limitNum) { 
    document.getElementById("Exceed").hidden = false;
    document.getElementById("Exceed").innerHTML = "Exceed Limit Number";
    document.getElementById("Full").hidden = true;
    document.getElementById("Number").hidden = true;
  }
  else if (data == limitNum) {
    document.getElementById("Exceed").hidden = true;
    document.getElementById("Full").hidden = false;
    document.getElementById("Full").innerHTML = "Room Full";
    document.getElementById("Number").hidden = true;
  }
  else {
    document.getElementById("Exceed").hidden = true;
    document.getElementById("Number").hidden = false;
    document.getElementById("Number").innerHTML = data;
    document.getElementById("Full").hidden = true;
  }
});

