<template>
  <div>
    <h3>Detail</h3>
    <div v-if="article">
      <p>제목 : {{ article.title }}</p>
      <p>내용 : {{ article.content }}</p>
      <p>게시일 : {{ article.created_at }}</p>
      <p>수정일 : {{ article.updated_at }}</p>
    </div>
  </div>
</template>

<script setup>
import axios from 'axios'
import { onMounted, ref } from 'vue';
import { useCounterStore } from '@/stores/counter'
import { useRoute } from 'vue-router';
const store = useCounterStore()
const route = useRoute()
const article = ref(null)
onMounted(() => {
  axios({
    method: 'get',
    url: `${store.API_URL}/api/v1/articles/${route.params.id}`
  })
    .then((res) => {
      console.log(res)
      article.value = res.data
    })
    .catch((err) => {
      console.log(err)
    })
})

</script>

<style>

</style>
