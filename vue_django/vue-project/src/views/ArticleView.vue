<template>
  <div>
    <h1>Article Page</h1>
    <RouterLink :to="{ name: 'CreateView' }">
      [CREATE!]
    </RouterLink>
    <ArticleList/>
  </div>
</template>

<script setup>
import { onMounted } from 'vue';
import { useCounterStore } from '@/stores/counter'
import { RouterLink } from 'vue-router';
import ArticleList from '../components/ArticleList.vue';

const store = useCounterStore()

onMounted(() => {
  store.getArticles()
})

</script>

<style>

</style>
