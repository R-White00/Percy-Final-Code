import { StatusBar } from 'expo-status-bar';
import { StyleSheet, Text, View, SafeAreaView, TextInput, TouchableOpacity,Pressable} from 'react-native';
import React, {useState, useEffect} from 'react';

import Ionicons from '@expo/vector-icons/Ionicons';
import { FontAwesome } from '@expo/vector-icons';
import { MaterialCommunityIcons } from '@expo/vector-icons';
import { MaterialIcons } from '@expo/vector-icons';
import BasicSpeedometer from 'react-native-speedometer-chart';
import { Line, Text as SvgText, G } from 'react-native-svg'
import { Button } from 'react-native-paper';
// import RNSpeedometer from 'react-native-speedometer'
import { initializeApp } from "firebase/app";
import { getAnalytics } from "firebase/analytics";
import {collection, getFirestore, onSnapshot, doc, setDoc}  from "firebase/firestore";
import Speedometer , {
  Background,
  Arc,
  Needle,
  Progress,
  Marks,
  Indicator,
  DangerPath,
} from 'react-native-cool-speedometer';

import { NavigationContainer } from '@react-navigation/native';
import { createNativeStackNavigator } from '@react-navigation/native-stack';
import loginScreen  from "./Screens/loginScreen.js"
import adminScreen  from "./Screens/adminScreen.js"
import guestScreen  from "./Screens/guestScreen.js"


const firebaseConfig = {
  apiKey: "AIzaSyCqnxinzpFRFkfIIShDvlw8uVr_ULMm2CA",
  authDomain: "percy-dashboard.firebaseapp.com",
  projectId: "percy-dashboard",
  storageBucket: "percy-dashboard.appspot.com",
  messagingSenderId: "396244618046",
  appId: "1:396244618046:web:61eecd0942620697c8fa6f",
  measurementId: "G-ETGNWVXGB0"
};

const app = initializeApp(firebaseConfig);
const analytics = getAnalytics(app);
const db = getFirestore(app);
const commandRef = doc(db, "Commands", "PercyCommands");
const deltaRef = doc(db, "Commands", "DeltaCommands");
const Stack = createNativeStackNavigator();
export default function App() {






  return (
    <NavigationContainer>
    <Stack.Navigator screenOptions={{ headerShown: false }}>
      <Stack.Screen name="loginScreen" component={loginScreen}/>
      <Stack.Screen name="adminScreen" component={adminScreen}/>

      <Stack.Screen name="guestScreen" component={guestScreen}/>

    </Stack.Navigator>
  </NavigationContainer>
  );
}

const styles = StyleSheet.create({
  container: {


flex: 1,
  },

 
  
});
