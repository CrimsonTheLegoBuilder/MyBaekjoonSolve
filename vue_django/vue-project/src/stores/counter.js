import { ref, computed } from 'vue'
import { defineStore } from 'pinia'
import axios from 'axios'

export const useCounterStore = defineStore('counter', () => {
  const articles = ref([

  ])
  const API_URL = 'http://127.0.0.1:8000'
  
  const getArticles = function() {
    axios({
      method:'get',
      url: `${API_URL}/api/v1/articles/`
    })
      .then((res) => {
        // console.log(res)
        articles.value = res.data
      })
      .catch((err) => {
        console.log(err)
      })
  }
  return { articles, API_URL, getArticles }
}, { persist: true })
