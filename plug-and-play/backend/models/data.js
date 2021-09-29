import  mongoose from 'mongoose' ;
const Schema = mongoose.Schema;

const dataSchema = new Schema(
    {
        temperature: {
            type: String,
            required: false
        },
        humid: {
            type: String,
            required: false
        },
        dist: {
            type: String,
            required: false
        }


    },{timestamps:true }
)



const Data  = mongoose.model('Data',dataSchema);
export default  Data;

