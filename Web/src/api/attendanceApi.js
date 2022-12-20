import axiosClient from "./axiosClient"

const attendanceApi = {
    getall(id){
        const url =`/checkin?date=${id}`
        return axiosClient.get(url);
    }
}
export default attendanceApi;